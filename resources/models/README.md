# Models

- UltraFace.onnx - Lightweight face detector. All information can be found [here](https://github.com/onnx/models/tree/master/vision/body_analysis/ultraface).
- YOLOX_*.onnx - Lightweight object detector. All information can be found [here](https://github.com/Megvii-BaseDetection/YOLOX).
- PSPNet_PascalVOC.onnx - Semantic segmentation model. Trained by MMSegmentation team with [this](https://github.com/open-mmlab/mmsegmentation/blob/master/configs/pspnet/pspnet_r50-d8_512x512_40k_voc12aug.py) config.
 Converted to ONNX by [pytorch2onnx](https://github.com/open-mmlab/mmsegmentation/blob/master/tools/pytorch2onnx.py), after that [simplified](https://github.com/daquexian/onnx-simplifier).
