This repo aims to be a comprehensive resource designed to break down a complex embedded system into simple, manageable parts. It demonstrates a clear separation of the key components involved in stepper motor control:

- Encoder for Position Feedback: Keeps track of motor position.
- PWM Timer for Step Pulse Generation: Manages the stepping rate.
- GPIO for Direction Control: Determines the motor’s rotational direction.
  
Each component is built and tested in isolation, allowing you to:

- Easily Debug and Improve: Isolating functionalities means you can pinpoint issues quickly and update specific modules without worrying about breaking the entire system.
- Learn Piece by Piece: The project is structured so that each part can operate as a standalone system. This makes it ideal for beginners who can test and learn step by step, eventually combining their knowledge to control a stepper motor.
- Encourage Modular Thinking: With clearly separated parts, modifying or enhancing one component doesn’t force you to rework the whole design. This makes it easier to experiment, iterate, and build upon each piece independently.

useful information: this project is built on top of auto-generated CubeMX code and leverages the PlatformIO project structure.
