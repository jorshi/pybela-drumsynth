You can install the dependencies either with pip (`pip install -r requirements.txt`) or v `uv`.
then jupyter notebook
then

```bash
docker pull pelinski/xc-bela-container:v1.1.0
docker run -it --name bela-container -e BBB_HOSTNAME=192.168.7.2 pelinski/xc-bela-container:v1.1.0
```

in another terminal

```bash
docker cp pybela-drumsynth bela-container:/sysroot/root/Bela/projects/
```

in docker

```bash
# in docker
cd /sysroot/root/Bela/projects/pybela-drumsynth # path to the project
mkdir build && cd build


```
