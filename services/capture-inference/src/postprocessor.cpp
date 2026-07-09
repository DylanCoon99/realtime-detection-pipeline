#include <algorithm>
#include "types.h"
#include "postprocessor.h"


float intersection_over_union(cv::Rect box1, cv::Rect box2) {
    cv::Rect intersection = box1 & box2;
    float intersection_area = intersection.area();
    float union_area = box1.area() + box2.area() - intersection_area;
    float iou = intersection_area / union_area;
    return iou;
}


Frame& Postprocessor::process(Frame& frame, std::vector<DetectionObject>& detections) {

    // TODO: perform Non-Maximum Suppression (NMS) on detections
    
    /* Algorithm
     
     - Exclude any detection that has confidence below threshold (already done in the detection step)
     
     - Sort the detections by confidence in descending order (Done)
     - Suppress
        - box with highest confidence is kept
        - Computer Intersection Over Union (IOU) for the top box and remaining objects
        - Any box that has IOU over certain threshold is a duplicate (disregard it)
        - Repeat until no candidates remain
     
    */
    
    // sort the detections by confidence in descending order
    std::sort(detections.begin(), detections.end(), [](const DetectionObject& a, const DetectionObject& b) {
        return a.confidence_score > b.confidence_score;
    });
    
    
    std::vector<DetectionObject> filtered_detections;

    while (!detections.empty()) {
        // keep the highest confidence detection
        filtered_detections.push_back(detections.front());
        const auto& kept = filtered_detections.back();
        detections.erase(detections.begin());

        // remove any remaining detection that overlaps too much with the kept one
        detections.erase(
            std::remove_if(detections.begin(), detections.end(),
                [&kept, this](const DetectionObject& d) {
                    return d.class_label == kept.class_label &&
                           intersection_over_union(kept.bounding_box, d.bounding_box) > nms_threshold_;
                }), detections.end()
        );
    }

    detections = std::move(filtered_detections);
    
    

    // draw bounding boxes and labels
    for (const auto& det : detections) {
        // green box, 2px thick
        cv::rectangle(frame.image, det.bounding_box, cv::Scalar(0, 255, 0), 2);

        // label with class name and confidence
        std::string label = det.class_label + " " +
            std::to_string(static_cast<int>(det.confidence_score * 100)) + "%";

        // background rectangle for label readability
        int baseline = 0;
        cv::Size text_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
        cv::Point label_origin(det.bounding_box.x, det.bounding_box.y - 5);

        // keep label on screen if box is near top edge
        if (label_origin.y - text_size.height < 0) {
            label_origin.y = det.bounding_box.y + text_size.height + 5;
        }

        cv::rectangle(frame.image,
            cv::Point(label_origin.x, label_origin.y - text_size.height),
            cv::Point(label_origin.x + text_size.width, label_origin.y + baseline),
            cv::Scalar(0, 255, 0), cv::FILLED);

        cv::putText(frame.image, label, label_origin,
            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
    }

    return frame;
}
