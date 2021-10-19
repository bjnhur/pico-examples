<!-- omit in toc -->
# Getting Azure IoT SDK 
- [1. Download Azure IoT SDK](#1-download-azure-iot-sdk)
- [2. Download submodules in SDK](#2-download-submodules-in-sdk)
  - [2.1. "c-utility" directory](#21-c-utility-directory)
    - [2.1.1. "c-utility/deps" directory](#211-c-utilitydeps-directory)
  - [2.2. "deps" directory](#22-deps-directory)
  - [2.3. "provisioning_client" directory](#23-provisioning_client-directory)
    - [2.3.1. "provisioning_client/deps" directory](#231-provisioning_clientdeps-directory)
  - [2.4. "umqtt" directory](#24-umqtt-directory)
## 1. Download Azure IoT SDK
- Recent release : https://github.com/Azure/azure-iot-sdk-c/releases/tag/LTS_07_2021_Ref01
- Download & Extract

## 2. Download submodules in SDK

After downloading SDK files, you need to download submodules as listed below:

![image](https://user-images.githubusercontent.com/6334864/137651943-2f71b3c5-9244-4791-b765-64aa461f18ce.png)

### 2.1. "c-utility" directory

Download files from below github repo, extract and rename directory name to "c-utility".

- https://github.com/Azure/azure-c-shared-utility/tree/f1f54ef5ef2b7e1be4bd750f17d1888bb6d07166

#### 2.1.1. "c-utility/deps" directory

There are 2 more submodules in "c-utility/deps" directory. Download files from below github repos, extract and rename directory names to "azure-macro-utils-c", "umock-c".

![image](https://user-images.githubusercontent.com/6334864/137650814-f33e6f72-14de-436b-9bf5-4428804682bd.png)
  - https://github.com/Azure/macro-utils-c/tree/5926caf4e42e98e730e6d03395788205649a3ada
  - https://github.com/Azure/umock-c/tree/504193e65d1c2f6eb50c15357167600a296df7ff

### 2.2. "deps" directory

Download files from below github repo, extract and rename directory name to "parson".

![image](https://user-images.githubusercontent.com/6334864/137651352-93ee939b-4d3e-471f-89b8-a3fc823e69a8.png)
- https://github.com/kgabis/parson/tree/8beeb5ea4da5eedff8d3221307ef04855804a920

### 2.3. "provisioning_client" directory

#### 2.3.1. "provisioning_client/deps" directory

Download files from below github repo, extract and rename directory name to "utpm".

![image](https://user-images.githubusercontent.com/6334864/137652055-d8797e40-e219-4c8d-9dbf-6a72e5c61393.png)

- https://github.com/Azure/azure-utpm-c/tree/fa17c595f799d6d54d1847dcca4399d65e3ec256

### 2.4. "umqtt" directory

Download files from below github repo, extract and rename directory name to "umqtt".

- https://github.com/Azure/azure-umqtt-c

Done!
