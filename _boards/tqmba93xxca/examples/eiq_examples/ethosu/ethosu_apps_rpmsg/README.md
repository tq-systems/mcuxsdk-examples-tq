<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany. 
-->

# Ethos-U RPMSG Demo

## Overview

This 'ethosu_apps_rpmsg' demo application runs an inference model on the NPU of the TQMa93xx. This demo processes the image `grace_hopper.bmp` and tries to identify the content.

## Building the Demo

This command builds the Ethos-U RPMSG demo for Cortex-M33. The artefacts will be stored in the mcuxsdk-examples-tq/build folder.

```
west build mcuxsdk-examples-tq/_boards/tqmba93xxca/examples/eiq_examples/ethosu/ethosu_apps_rpmsg/ --board tqmba93xxca -Dcore_id=cm33 -DBINARY_DIR=mcuxsdk-examples-tq/build  -DCUSTOM_BOARD_ROOT="mcuxsdk-examples-tq/_boards" --config=release --pristine
```

## Running the Demo

This example is built as `.elf` and needs to be installed in `/usr/lib/firmware/ethosu_firmware`. In the Linux command line, users can follow the below steps to perform inference using the model:

Copy the `label.txt` and the input picture `grace_hopper.bmp` from the TensorFlow example folder.

```
cd /usr/bin/ethosu/examples
cp ../../tensorflow-lite-2.19.0/examples/labels.txt ./
cp ../../tensorflow-lite-2.19.0/examples/grace_hopper.bmp ./
```
Note: Tensorflow-lite versions may differ.

Compile the model for Ethos-U using the Vela tool by reusing the model mobilenet_v1_1.0_224_quant.tflite from /usr/bin/tensorflow-lite-2.19.0/examples/.
If running successfully, an optimized Vela model mobilenet_v1_1.0_224_quant_vela.tflite is generated in the output folder.

```
vela ../../tensorflow-lite-2.19.0/examples/mobilenet_v1_1.0_224_quant.tflite
```

Run the model with the inference API.

```
./inference_runner -d /dev/ethosu0 -n ./output/mobilenet_v1_1.0_224_quant_vela.tflite -i grace_hopper.bmp -l labels.txt -o output.txt
```

The `inference_runner` will load the Cortex-M33 firmware and start it. The following message will be seen in the Cortex-M33 UART terminal.

```
Version:
Branch: feature-tqma93-npu
This is a demo application for the MBa93xxCA device.
Initialize Arm Ethos-U
ETHOS-U FreeRTOS RTOS API Demo...
RPMSG_LITE is link up
Nameservice sent, ready for incoming messages...
```

When the example runs, the log and inference result appear on the terminal as shown below:

```
Send Ping
Send version request
Send capabilities request
Capabilities:
        version_status:1
        version:{ major=0, minor=0, patch=0 }
        product:{ major=6, minor=0, patch=0 }
        architecture:{ major=1, minor=0, patch=6 }
        driver:{ major=0, minor=16, patch=0 }
        macs_per_cc:8
        cmd_stream_version:0
        custom_dma:false
Create network
Create inference
Wait for inference
Inference status: ok
Output size: 1001

Detected: military uniform, confidence:72
```
