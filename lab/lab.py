from ultralytics import YOLO                                                                                                     

# Load pretrained model                                                                                                          
model = YOLO("yolov8n.pt")  # nano (fastest, least accurate)                                                                   
# model = YOLO("yolov8s.pt")  # small (good balance)                                                                             
                                                                                                                   
# Export to ONNX for your C++ pipeline                                                                                           
model.export(format="onnx")