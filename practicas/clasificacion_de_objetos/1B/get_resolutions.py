import cv2
import glob

for video_file in glob.glob('*.mp4'):
    cap = cv2.VideoCapture(video_file)
    if not cap.isOpened():
        print(f"Error opening video file: {video_file}")
        continue
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    print(f"{video_file}: {width}x{height}")
    cap.release()
