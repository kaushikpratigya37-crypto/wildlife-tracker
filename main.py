import cv2
import time

from arduino.app_utils import App


# Camera
camera = cv2.VideoCapture(0)

# Previous frame
previous_frame = None

# How sensitive the motion detection is
MOTION_THRESHOLD = 5000


def loop():

    global previous_frame

    # Read camera frame
    success, frame = camera.read()

    if not success:
        print("Camera not detected")
        time.sleep(1)
        return

    # Convert frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Reduce noise
    gray = cv2.GaussianBlur(gray, (21, 21), 0)

    # First frame
    if previous_frame is None:
        previous_frame = gray
        return

    # Compare current frame with previous frame
    difference = cv2.absdiff(previous_frame, gray)

    # Create binary image
    _, threshold = cv2.threshold(
        difference,
        25,
        255,
        cv2.THRESH_BINARY
    )

    # Count changed pixels
    motion_pixels = cv2.countNonZero(threshold)

    if motion_pixels > MOTION_THRESHOLD:

        print("🐾 WILDLIFE MOTION DETECTED!")

        # Capture image
        filename = "wildlife_capture.jpg"
        cv2.imwrite(filename, frame)

        print("📸 Image captured:", filename)

        # Small delay so multiple photos aren't captured immediately
        time.sleep(2)

    else:
        print("No motion detected")

    # Save current frame for next comparison
    previous_frame = gray

    time.sleep(0.1)


App.run(user_loop=loop)
