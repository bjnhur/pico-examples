<!-- omit in toc -->
# iothub_ll_c2d_sample application
- [1. Prepare IoT Hub and Device for this example](#1-prepare-iot-hub-and-device-for-this-example)
- [2. Run the example](#2-run-the-example)
  - [2.1. Edit main.c](#21-edit-mainc)
  - [2.2. Build & Copy uf2 file into your pico device](#22-build--copy-uf2-file-into-your-pico-device)
  - [2.3. Result](#23-result)


# 1. Prepare IoT Hub and Device for this example
**[MUST]** First you should follow up instructions from [_1_APP_TELEMETRY_manual.md](https://github.com/bjnhur/pico-examples/blob/main/_1_APP_TELEMETRY_manual.md)

# 2. Run the example 
## 2.1. Edit main.c
![image](https://user-images.githubusercontent.com/6334864/137408809-e1141483-42b6-4e09-8b9b-82ed77d70109.png)

## 2.2. Build & Copy uf2 file into your pico device
![image](https://user-images.githubusercontent.com/6334864/137408816-6f826c4e-e785-44e5-bc81-f35d04ce4d86.png)

## 2.3. Result
This example has 2 functions:
1. Send 3 message same as  [_1_APP_TELEMETRY_manual.md](https://github.com/bjnhur/pico-examples/blob/main/_1_APP_TELEMETRY_manual.md)

    > First, you get "function 1" result as below in "Serial Terminal" & "Azure IoT Explorer" window.

![image](https://user-images.githubusercontent.com/6334864/137409633-fbc194ca-5f89-4517-85af-817e1a53e5db.png)
![image](https://user-images.githubusercontent.com/6334864/137409743-0de157bb-a37b-42ff-ae7e-f1a3e05188ca.png)


2. Recv 3 C2D (Cloud to Device) message.

    > For "function 2 C2D", you can use "Azure IoT Explorer" program as follows:

![image](https://user-images.githubusercontent.com/6334864/137410000-726f4456-d81e-4154-9d86-cd3418d23f99.png)
![image](https://user-images.githubusercontent.com/6334864/137410174-51ea1f10-cc83-4187-aed4-44575ae90c11.png)

    > Then, you can see the received message from "Azure Cloud" on your "Serial Terminal" window as below:

![image](https://user-images.githubusercontent.com/6334864/137410288-2f3a9f04-fab0-4c40-be9d-a502a76769f9.png)

    > This example waits for 3 messages from cloud.

![image](https://user-images.githubusercontent.com/6334864/137410439-3fd284c9-87ef-4e42-8862-634e13363111.png)
![image](https://user-images.githubusercontent.com/6334864/137410450-3b02ac64-d754-418b-8d06-90ea390d2aa2.png)
![image](https://user-images.githubusercontent.com/6334864/137410492-b12935bd-483a-452c-ae62-9cc287830023.png)
![image](https://user-images.githubusercontent.com/6334864/137410553-c7ef1408-e074-4dc8-b0db-6f1deb8b460a.png)
