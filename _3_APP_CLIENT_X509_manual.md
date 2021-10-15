- [1. Prepare IoT Hub and Device for this example](#1-prepare-iot-hub-and-device-for-this-example)
  - [1.1. Developer PC - Generate Device self-signed certificates](#11-developer-pc---generate-device-self-signed-certificates)
  - [1.2. Developer PC - SDK setting](#12-developer-pc---sdk-setting)
  - [1.3. Developer PC - Build](#13-developer-pc---build)
  - [1.4. Developer PC - Serial terminal open](#14-developer-pc---serial-terminal-open)
  - [1.5. Azure portal](#15-azure-portal)
  - [1.6. Developer PC - Azure IoT Explorer (preview) setting](#16-developer-pc---azure-iot-explorer-preview-setting)
- [2. Run the example code](#2-run-the-example-code)
  - [2.1. copy main.uf2 file into your Pico board](#21-copy-mainuf2-file-into-your-pico-board)
  - [2.2. Serial terminal log](#22-serial-terminal-log)
  - [2.3. Azure IoT Explorer (preview) log](#23-azure-iot-explorer-preview-log)
# 1. Prepare IoT Hub and Device for this example
## 1.1. Developer PC - Generate Device self-signed certificates

**[MUST]** Please follow up [tutorial-x509-self-sign](https://docs.microsoft.com/en-us/azure/iot-hub/tutorial-x509-self-sign)

For your reference, prepare example log as below:
Notice! device ID = **"W5100S_EVB_PICO_X509"**

```bash
MINGW64 ~
$ mkdir certi

MINGW64 ~
$ cd certi/

MINGW64 ~/certi
$ openssl genpkey -out device1.key -algorithm RSA -pkeyopt rsa_keygen_bits:2048
......................................+++++
.........................................................+++++

MINGW64 ~/certi
$ openssl req -new -key device1.key -out device1.csr
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:.
State or Province Name (full name) [Some-State]:.
Locality Name (eg, city) []:.
Organization Name (eg, company) [Internet Widgits Pty Ltd]:.
Organizational Unit Name (eg, section) []:.
Common Name (e.g. server FQDN or YOUR name) []:W5100S_EVB_PICO_X509
Email Address []:

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:
An optional company name []:

MINGW64 ~/certi
$ openssl req -text -in device1.csr -noout
Certificate Request:
    Data:
        Version: 1 (0x0)
        Subject: CN = W5100S_EVB_PICO_X509
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                RSA Public-Key: (2048 bit)
                Modulus:
                    00:bb:ba:cb:62:7a:ce:ac:4d:ff:88:c7:1a:ad:6a:
                    b4:6b:83:cc:30:74:94:7b:d2:8c:ed:6f:37:bf:c2:
...
                    ff:17:35:fb:78:d8:a8:31:04:a6:dd:89:f5:d6:fd:
                    a2:8e:e2:b3:62:d4:96:f2:9b:80:b5:22:4a:e2:6f:
                    88:e3
                Exponent: 65537 (0x10001)
        Attributes:
            a0:00
    Signature Algorithm: sha256WithRSAEncryption
         7e:de:0e:58:a6:44:c4:a6:76:12:be:a5:e0:80:35:90:ec:cb:
...
         73:ca:29:5f:36:d9:cd:1c:1e:34:98:c3:9a:a8:93:ef:28:f4:
         a9:45:f9:4e

MINGW64 ~/certi
$ openssl x509 -req -days 365 -in device1.csr -signkey device1.key -out device1.crt
Signature ok
subject=CN = W5100S_EVB_PICO_X509
Getting Private key

MINGW64 ~/certi
$ openssl genpkey -out device2.key -algorithm RSA -pkeyopt rsa_keygen_bits:2048
............................................................................................................................+++++
.....+++++

MINGW64 ~/certi
$ openssl req -new -key device2.key -out device2.csr
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:.
State or Province Name (full name) [Some-State]:.
Locality Name (eg, city) []:.
Organization Name (eg, company) [Internet Widgits Pty Ltd]:.
Organizational Unit Name (eg, section) []:.
Common Name (e.g. server FQDN or YOUR name) []:W5100S_EVB_PICO_X509
Email Address []:

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:
An optional company name []:

MINGW64 ~/certi
$ openssl x509 -req -days 365 -in device2.csr -signkey device2.key -out device2.crt
Signature ok
subject=CN = W5100S_EVB_PICO_X509
Getting Private key

MINGW64 ~/certi
$ openssl x509 -in device1.crt -noout -fingerprint
SHA1 Fingerprint=F3:61:90:1F:B5:76:xx:xx:xx:xx:9B:51:4F:51

MINGW64 ~/certi
$ openssl x509 -in device2.crt -noout -fingerprint
SHA1 Fingerprint=09:75:4F:7F:14:xx:xx:xx:xx:38:8B:D5:0D

MINGW64 ~/certi
$

```

- Provide the Device ID that matches the subject name of your two certificates. In this example, "W5100S_EVB_PICO_X509"
- Select the X.509 Self-Signed authentication type.
- Paste the hex string thumbprints that you copied from your device primary and secondary certificates. Make sure that the hex strings have no colon delimiters.

## 1.2. Developer PC - SDK setting

- Get the key value from files _(device1.crt, device1.key)_ as below:
![image](https://user-images.githubusercontent.com/6334864/137420283-440321ad-6656-4cff-9153-a26188df6d2c.png)

- Edit sample_certs.c with generated certificates as upper. Connection string for this example is "HostName=twarelabhub.azure-devices.net;DeviceId=W5100S_EVB_PICO_X509;x509=true"
  
> ./pico-azure-iot-sdk-c/application/sample_certs.c
![image](https://user-images.githubusercontent.com/6334864/137420358-922cca25-b3b8-4b73-9298-7423d5f4c28a.png)

- Select example in main.c 
> ./pico-azure-iot-sdk-c/application/main.c
![image](https://user-images.githubusercontent.com/6334864/137421611-ddf540ee-e699-42c3-a229-9265088e73f5.png)

## 1.3. Developer PC - Build

![image](https://user-images.githubusercontent.com/6334864/137421861-ada5ee2d-c153-4d75-bfb2-b8641f4d4919.png)

## 1.4. Developer PC - Serial terminal open
![image](https://user-images.githubusercontent.com/6334864/137317966-b9f63168-e011-4a0a-a3b1-345d1e847304.png)

## 1.5. Azure portal 
![image](https://user-images.githubusercontent.com/6334864/137417701-28ded168-7bdd-4e9c-a89d-d4cc7304440c.png)
![image](https://user-images.githubusercontent.com/6334864/137418264-3ec14375-a3ea-40f4-9dc6-8292e2966a76.png)
![image](https://user-images.githubusercontent.com/6334864/137418465-485410ed-6c9f-4744-853a-ed42818c2772.png)
![image](https://user-images.githubusercontent.com/6334864/137418475-4b487e14-15b7-4bb8-913c-ab9009eaf6a3.png)

## 1.6. Developer PC - Azure IoT Explorer (preview) setting
![image](https://user-images.githubusercontent.com/6334864/137422040-1f15781b-69a3-4c23-a92e-adfc452acd01.png)
![image](https://user-images.githubusercontent.com/6334864/137422225-4d735152-e220-4801-918e-12532ef9fd4a.png)
![image](https://user-images.githubusercontent.com/6334864/137422316-7b5916d3-1851-4344-9776-b6ce193e0089.png)

# 2. Run the example code

## 2.1. copy main.uf2 file into your Pico board
![image](https://user-images.githubusercontent.com/6334864/137318763-14d23305-af22-45d1-ab43-4143b50b658c.png)

## 2.2. Serial terminal log
![image](https://user-images.githubusercontent.com/6334864/137422831-f77bee77-d372-44d4-ac81-8b798710be28.png)
![image](https://user-images.githubusercontent.com/6334864/137422966-984ffb83-e579-44e0-911e-7f45572afec8.png)

## 2.3. Azure IoT Explorer (preview) log
![image](https://user-images.githubusercontent.com/6334864/137423061-fbb80b7f-9d27-49a9-bff2-b1377ac3ac39.png)
