<h1>Cosmo Robot</h1>

<!-- ![Cosmo Robot](Images/in_operation.jpg) -->
<img src="Images/in_operation.jpg" width="70%">


<h2>Project Summary</h2>

  <p>This project was based on Arduino Mega 2560 and developed for the robot competition held under the Semester 3 module EN2533: Robot Design and Competition. "Cosmo" robot is designed to showcase various capabilities, including precise line following, obstacle avoidance during line following, navigating ramps at 20 - 30 degrees, interacting with objects using a mechanical arm, sound sensitivity, color detection, and maze-solving capabilities. Additionally, it was programmed to avoid other robots in the arena such as the guard robot, on its way to the final destination.</p>

  <h2>Features</h2>

  <ul>
      <li>Precise line following.</li>
      <li>Obstacle avoidance during line following.</li>
      <li>Ramp navigation (20 - 30 degrees).</li>
      <li>Mechanical arm for interacting with objects.</li>
      <li>Sound sensitivity.</li>
      <li>Color detection.</li>
      <li>Maze solving capabilities.</li>
      <li>Avoiding other robots (Guard Robot).</li>
  </ul>

  <h2>Components Used</h2>

  <ul>
      <li>Arduino Mega 2560.</li>
      <li>Aptinex Reykha IR array and additional IR sensor modules.</li>
      <li>100RPM N20 motors.</li>
      <li>L298 motor driver.</li>
      <li>HY-SRF05 ultrasonic sensors.</li>
      <li>VL53L0X Time-Of-Flight (TOF) sensor.</li>
      <li>TCS3472 RGB color sensor.</li>
      <li>Sound (decibel) sensor.</li>
      <li>3D printed mechanical arm with MG90 metal servo motor.</li>
  </ul>

  <h2>Design Process</h2>

  <ul>
    <li>VS Code and Arduino IDE for programming the robot in C++ language.</li>
    <li>Blender and SOLIDWORKS for robot structure and chassis design.</li>
  </ul>

 <h2>Capable Tasks of the Robot </h2>

  <img src="Images/in_operation.jpg" width="70%">




1. All robots will commence their journey from the designated starting location.
2. If you are unable to complete the task by starting from the designated starting location,
you can start from a checkpoint that is not the designated starting checkpoint with a
penalty.
3. While following the line, the robot shouldn’t collide with any object that isn’t specifically
mentioned in the document.
4. When the robot is navigating from checkpoint - 3 to checkpoint - 4, it must go over the
bridge section and follow the line afterward. There will be line on the bridge, as well.
5. You can use any mechanism to find the path to reach checkpoint - 5.
6. Pick and place the colored box in the relevant colored space.
7. The robot should freeze when sounds emit from the sound tower.
8. The robot shouldn’t collide with the guard robot.
9. The game is considered finished when the robot completes all six sub tasks, sequentially.


<h2> Game Field (Arena) Specifications </h2>

1. The game arena is characterized by dimensions of 8 feet by 8 feet as shown in figure 14.
2. Within the arena, you will find two sections dedicated to line following, obstacles avoiding,
motion with a buzzer’s sound, a bridge, a robot guard moving section, a designated starting
point, and a specified stopping point.
3. The area designated for line following and obstacles avoiding is marked by white lines, 30
mm in width, on a non-reflective matte black surface.
4. The square boxes (which may not always be positioned at the center) are 5 cm by 5 cm
in size.
5. White squares and colored squares, located on the floor are 15 cm by 15 cm in size.
6. The wall/obstacle has a height of 10 cm and is colored white.
7. Specific dimensions for the bridge section are included in the figure 12 (length measurements are in centimeters ).
8. The wording ”checkpoint X” will not be on the arena. They are shown in the figures just
for understanding purposes, only.

<h2>Robot Specifications </h2>

1. Autonomous Operation: The robot must operate entirely autonomously. Once the
robot initiates its actions, team members are prohibited from making any contact with it.
2. Robot Dimensions: The robot’s dimensions should enable it to fit comfortably within a
250mm x 250mm box (length x width). There are no height restrictions. The robot must
be started using a single onboard switch. Teams may use separate switches for restarting
and for starting from checkpoints, which must be presented to the judges prior to the run.
3. Stability Requirement: The robot must demonstrate stability and stand independently
at the starting zone when the race begins. Failure to meet this criterion will result in
disqualification.
4. Expanding Mechanism: The robot is allowed to expand during its run as long as no
damage is inflicted on the arena. It is strictly prohibited to leave behind any parts or marks
while moving within the arena. Non-compliance will lead to potential disqualification for
the team.
5. Unity of the Bot: The robot cannot split into multiple units during gameplay. Any
machines or units that are in contact with each other or are within the start zone will be
considered as a single entity.
6. Use of Components: Teams are permitted to utilize pre-made microcontroller boards
and sensor kits. However, the incorporation of wireless modules, ready-made Lego kits,
or similar assemblies are not allowed. The use of off-the-shelf kits is prohibited.
7. Simple Starting Procedure: The robot’s starting procedure should be straightforward
and devoid of any manual force or directional impuls
  
  <h2>Contributing</h2>

  <p>Feel free to contribute to the project by submitting bug reports, feature requests, or pull requests. Your contributions are highly appreciated!</p>



