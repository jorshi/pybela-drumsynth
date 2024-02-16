# pybela-drumsynth

This is a Bela project that accompanies the [pybela + pytorch tutorial](https://github.com/pelinski/pybela-pytorch-xc-tutorial) by Teresa Pelinski. 

In this project I explore how to incorporate pybela into an existing Bela project for audio-driven drum synthesis. The main idea of that project is to use audio from a microphone to control a drum synthesizer using onset detection and audio feature extraction -- which map to synthesis parameter updates.

The original project is in the `drumsynth-original` folder, which can be zipped and loaded into the Bela IDE.

## Getting Started

1) First, you'll need to have your Bela setup according to the [pybela + pytorch tutorial](https://github.com/pelinski/pybela-pytorch-xc-tutorial). This includes the experimental image and the Docker container for cross-compilation. Don't worry about starting the Jupyter server within Docker though.

2) Then on your computer (outside of Docker)
```
git clone https://github.com/jorshi/pybela-drumsynth.git
```

3) Install python requirements
```
cd pybela-drumsynth
pip install -r requirements.txt
```

## pybela and model training

1) Start a Jupyter server:
```
jupyter notebook
```
And open `drumsynth_pybela.ipynb` -- follow the tutorial in the notebook. You'll need to have your Bela connected at this point.

In the notebook you'll copy the `pybela-drumsynth` code over to Bela, log audio features from Bela to Python, train a model, and copy over to Bela. There are also instructions on compiling [wip] inference code and running that on Bela.
