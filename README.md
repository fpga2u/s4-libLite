# s4
A股相关工具库 - 基础库-轻量版(目前仅供L2live使用)

争取跨平台，但目前先在Windows(VS2017/2019)上开发/测试，不定期在Linux(CentoOS8 gcc8.3)上编译测试

## Todo list


## 要求 requirement

- Visual Studio 2017 or 2019 / gcc-8.3 (c++17)
- cmake 3.9
- python 3.7

## 安装 install

### get source:
```shell
git clone https://github.com/fpga2u/s4-libLite.git
git submodule update --init --recursive
```

### build
- windows
```shell
cd s4/build

# Call VS Developer Command Prompt
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

cmake -A x64 ..

#build：
cmake --build . --config Release

#install：
cmake --build . --target install --config Release
```

- linux
```shell
cd s4\build

cmake ..

#build：
cmake --build . --config Release

#install：
sudo cmake --build . --target install --config Release
```

* 若Linux 出现 Failed to find "GL/gl.h" in "/usr/include/libdrm" :
  sudo yum install mesa-libGL-devel mesa-libGLU-devel

## Thirdparty && Reference

- asio : https://github.com/chriskohlhoff/asio
- logger : https://github.com/gabime/spdlog
- json : https://github.com/nlohmann/json
- non-lock queue :  https://github.com/cameron314/concurrentqueue
                    https://github.com/cameron314/readerwriterqueue
