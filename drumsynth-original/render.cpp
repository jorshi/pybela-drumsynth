/**
 * Audio Driven Drum Synthesis
 * MAP Final Project - 2023
 * Jordie Shier
 * 
 * Snare drum synthesizer based on early drum machines that
 * can be triggered and controlled in response to audio
 * input from a microphone.
 */
#include <Bela.h>
#include <libraries/Scope/Scope.h>
#include <libraries/Gui/Gui.h>
#include <libraries/GuiController/GuiController.h>
#include <algorithm>
#include <cmath>

#include <SnareDrum.h>
#include <DrumController.h>

// Bela oscilloscope
Scope gScope;

// Browser-based GUI to adjust parameters
Gui gGui;
GuiController gGuiController;

// Synthesis & Synth Controller
SnareDrum drum;
DrumController controller(drum);

// Globals to help with GUI parameter mapping
int paramStartIndex;
int numParams;

bool setup(BelaContext *context, void *userData)
{
	// Initialise the scope
	gScope.setup(3, context->audioSampleRate);
	
	// Setup processors
	drum.prepare(context->audioSampleRate);
	controller.prepare(context->audioSampleRate);
	
	// Initialize drum params
	auto& noise = drum.getNoise();
	noise.setDecay(0.25);
	noise.setColor(0.25);
	noise.setTone(0.75);
	
	auto& tonal = drum.getTonal();
	tonal.setDecay(0.25);
	tonal.setTuning(0.5);
	
	// Set up the GUI
	gGui.setup(context->projectName);
	gGuiController.setup(&gGui, "Drum Control");	
	
	// Arguments: name, default value, minimum, maximum, increment
	gGuiController.addSlider("Listen", 0.0, 0.0, 1.0, 1.0);
	gGuiController.addSlider("Trigger Threshold", 16.0, 0.0, 30.0, 0);
	
	paramStartIndex = 2;
	numParams = 7;
	gGuiController.addSlider("Tonal Decay", 0.30, 0.0, 1.0, 0);
	gGuiController.addSlider("Tonal Decay Energy Mod", 0.0, -1.0, 1.0, 0);
	gGuiController.addSlider("Tonal Decay Spectral Mod", 0.0, -1.0, 1.0, 0);
	
	gGuiController.addSlider("Tonal Tuning", 0.60, 0.0, 1.0, 0);
	gGuiController.addSlider("Tonal Tuning Energy Mod", 0.0, -1.0, 1.0, 0);
	gGuiController.addSlider("Tonal Tuning Spectral Mod", 0.0, -1.0, 1.0, 0);
	
	gGuiController.addSlider("Noise Decay", 0.30, 0.0, 1.0, 0);
	gGuiController.addSlider("Noise Decay Energy Mod", 0.0, -1.0, 1.0, 0);
	gGuiController.addSlider("Noise Decay Spectral Mod", 0.0, -1.0, 1.0, 0);
	
	gGuiController.addSlider("Noise Tone", 0.92, 0.0, 1.0, 0);
	gGuiController.addSlider("Noise Tone Energy Mod", 0.0, -1.0, 1.0, 0);
	gGuiController.addSlider("Noise Tone Spectral Mod", 0.0, -1.0, 1.0, 0);	

	gGuiController.addSlider("Noise Color", 0.75, 0.0, 1.0, 0);
	gGuiController.addSlider("Noise Color Energy Mod", 0.0, -1.0, 1.0, 0);
	gGuiController.addSlider("Noise Color Spectral Mod", 0.0, -1.0, 1.0, 0);
	
	gGuiController.addSlider("Mix Ratio", 0.5, 0.0, 1.0, 0);
	gGuiController.addSlider("Mix Ratio Energy Mod", 0.0, -1.0, 1.0, 0);
	gGuiController.addSlider("Mix Ratio Spectral Mod", 0.0, -1.0, 1.0, 0);

	gGuiController.addSlider("Gain", 0.5, 0.0, 1.0, 0);
	gGuiController.addSlider("Gain Energy Mod", 0.0, -1.0, 1.0, 0);
	gGuiController.addSlider("Gain Spectral Mod", 0.0, -1.0, 1.0, 0);
	
	return true;
}

void render(BelaContext *context, void *userData)
{
	// Whether we should listen and update parameter mapping ranges		
	int listen = static_cast<int>(gGuiController.getSliderValue(0));
	controller.shouldListen(listen);
	
	// Triggering threshold
	float threshold = gGuiController.getSliderValue(1);
	controller.getOnsetDetector().updateParameters(threshold, threshold / 2.0, context->audioSampleRate / 20.0);
	
	// Get synth and mapping parameters from the GUI sliders
	auto& mapping = controller.getParameterMapping();
	for (int i = 0; i < numParams; ++i)
	{
		int index = (i * 3) + paramStartIndex;
		float paramVal = gGuiController.getSliderValue(index);
		float paramModA = gGuiController.getSliderValue(index + 1);
		float paramModB = gGuiController.getSliderValue(index + 2);

		mapping.parameterValues[i] = paramVal;
		mapping.parameterModAmount[i][0] = paramModA;
		mapping.parameterModAmount[i][1] = paramModB;
	}
	
	int numAudioFrames = context->audioFrames;
	for(int n = 0; n < numAudioFrames; n++) {
		// Read from the microphone and process with the synth controller
		float x = audioRead(context, n, 0);
		controller.process(x);
		
		// Get the next audio sample from the drum
		float y = drum.process();
		
		// Main audio processing loop
		for(int channel = 0; channel < context->audioOutChannels; channel++){
			audioWrite(context, n, channel, y);
		}
		
		// Scope for evaluation
		//gScope.log(drum.getGain(), drum.getMix(), x);
	}
}

void cleanup(BelaContext *context, void *userData)
{

}