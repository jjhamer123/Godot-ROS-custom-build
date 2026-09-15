# Godot-ROS-custom-build

A custom build of the Godot 4 / ROS2 integration, forked and extended by Jack Hamer for real-time robot control and digital-shadow work. This repository ships the C++ Godot-ROS module (targeting ROS2 Jazzy, with the joint-level `JointStatePublisher`/`JointJogController` classes described below) together with the [MOVER6-Simulation-in-Godot-from-URDF](https://github.com/jjhamer123/MOVER6-Simulation-in-Godot-from-URDF) project as a git submodule.

This is a general-purpose **simulation and real-time digital shadow platform for robot arms**, not an arm-specific build: the joint-level ROS2 interface and the `RosArmBridge` bridge script work off joint names/limits supplied by a generic `RobotJointController`, so any URDF-imported arm rig can be swapped in. The bundled Mover6 submodule is the current reference implementation and example arm, not the only one this build supports.

The original 4WS4WD swerve-drive vehicle demo has been removed from this repository — the intent going forward is for the Godot side to run as a real-time simulation and/or digital shadow of a robot arm, not a general-purpose vehicle sim.

This project fuses the powerful open-source game engine Godot 4 with ROS2 (Robot Operating System). Godot's modular source code fits seamlessly into the ROS2 pipeline, providing a platform for developing and testing robotics algorithms — and for mirroring a physical robot's real-time state — inside a game engine rather than a dedicated robotics simulator.

## Repository structure

- `src/godot_custom_modules/godot_ros/` — the native C++ Godot module that talks to ROS2 (built into the Godot editor/engine itself). This is the part that provides `JointStatePublisher` and `JointJogController` to GDScript.
- `src/Godot_MOVER6_simulation_game/` — **git submodule** pointing at [MOVER6-Simulation-in-Godot-from-URDF](https://github.com/jjhamer123/MOVER6-Simulation-in-Godot-from-URDF). This is the actual Godot project you open: a URDF-imported Mover6 arm rig (via the `godot_urdf` addon), driven over ROS2 by the classes from the module above. It's the current example arm — since `RosArmBridge` and the joint-level ROS2 classes take joint names/limits from a generic `RobotJointController` rather than anything Mover6-specific, a different URDF-imported arm submodule could be swapped in here just as easily.

## Credits / lineage

- Original idea and starting point: [flynneva/godot_ros](https://github.com/flynneva/godot_ros) ([youtube](https://youtu.be/4d_pCm2d-Gs?si=bfxK0tHN8EW_-YoN)).
- Ported to Godot 4.1–4.4 and built out the original 4WS4WD simulation game and Blender pipeline: [nordstream3/Godot-4-ROS2-integration](https://github.com/nordstream3/Godot-4-ROS2-integration).
- This repository (`jjhamer123/Godot-ROS-custom-build`) takes over from there: it ports the module build to ROS2 Jazzy, adds joint-level pub/sub classes for driving arm joints generically, and ships the [Mover6 URDF simulation](https://github.com/jjhamer123/MOVER6-Simulation-in-Godot-from-URDF) as an example arm project in place of the original vehicle demo.

## Changes in this fork

Compared to `nordstream3/Godot-4-ROS2-integration`:

- **Targets ROS2 Jazzy** instead of Humble. `SCsub` now defaults to `ros_distro = "jazzy"` and `ros_dir = "/opt/ros/jazzy"`.
- **Supports merged ROS2 installs.** `getIncludeDirs()` in `SCsub` now also walks an `include/<package>` layout (as produced by a merged/underlay install), in addition to the original per-package `<pkg>/include/<pkg>` layout, so the module can find headers regardless of how the ROS2 workspace was built.
- **Adds joint-level ROS2 interfaces** — `JointStatePublisher` and `JointJogController` (see below) — registered alongside the existing `ViewPort` and `CmdListener` classes in `register_types.cpp`. These are arm-agnostic: they move plain joint name/position/velocity arrays, not anything specific to one robot.
- **Removes the 4WS4WD vehicle demo** (`src/Godot_4WS4WD_simulation_game/`), including its Blender-based warehouse scene and swerve-drive scripts, since the focus is now robot arms rather than mobile/wheeled robots.
- **Adds an example arm sim as a submodule** at `src/Godot_MOVER6_simulation_game/`, pointing at [MOVER6-Simulation-in-Godot-from-URDF](https://github.com/jjhamer123/MOVER6-Simulation-in-Godot-from-URDF). This is a URDF-imported arm rig (using the `godot_urdf` addon) rather than a hand-built Blender scene. Any other URDF-imported arm exposing the same `RobotJointController` interface could be substituted here.

## Jolt Physics Engine

Godot 4.x comes with a built-in physics engine, which does not meet the high precision and performance requirements needed for advanced robotics simulations. To address this, this build uses the **Jolt Physics Engine**, a native extension for Godot that provides a more robust and accurate physics simulation environment.

Jolt is known for its high performance and stability, making it well-suited for simulations involving complex physical interactions. For more details on the Jolt Physics Engine and its integration with Godot, refer to the following resources:

- [Jolt Physics Engine for Godot](https://github.com/godot-jolt/godot-jolt)
- [Discussion on Godot 4's Default Physics Engine](https://www.reddit.com/r/godot/comments/16p90a1/godot_4_default_physics_engine_is_a_complete_mess/)

## Features

- **Godot Module for ROS2**: Seamlessly integrate Godot 4 with ROS2 for advanced robotics simulations.
- **Jolt Physics Engine**: Utilizes the Jolt Physics Engine for enhanced simulation accuracy and stability.
- **Robot arm simulation & digital shadow, for any URDF-imported arm**: `RosArmBridge` (`ros_2_arm.gd`) drives an arm's joints purely through a generic `RobotJointController` interface (joint names, angles, limits), so it isn't tied to any one robot. It supports both a **Simulation** mode (incoming `/JointJog` commands actually move the arm's physics) and a **Shadow** mode (the arm just reports its current state, ignoring incoming commands), with per-joint sign overrides and a max-velocity setting to tune it to a given arm's conventions. The bundled `Godot_MOVER6_simulation_game` submodule is the current example arm this is exercised against — see that project's own README for the URDF import and rig setup.
- **Joint State Publisher**: A `JointStatePublisher` GDScript-exposed class that publishes `sensor_msgs/msg/JointState` on `/joint_states`, so any joint values driven inside Godot can be observed by the rest of the ROS2 stack.
- **Joint Jog Controller**: A `JointJogController` GDScript-exposed class that publishes and subscribes to `control_msgs/msg/JointJog` on `/JointJog`, letting Godot both command external joints and react to jog commands coming from ROS2.

## New ROS2 classes

Both classes live in `src/godot_custom_modules/godot_ros/{include/godot_ros/demos,src/demos}` and are registered as native Godot classes, so they can be instantiated directly from GDScript with no extra bindings. They only deal in plain joint name/position/velocity arrays, so they work the same regardless of which arm (or robot) is on the other end. The Mover6 submodule's `ros_2_arm.gd` (`RosArmBridge`) is the reference consumer of both, and is a good template for wiring up a different arm.

### `JointStatePublisher`

Publishes `sensor_msgs/msg/JointState` messages on `/joint_states`.

```gdscript
var joint_pub = JointStatePublisher.new()

func _physics_process(delta):
    joint_pub.spin_some()
    joint_pub.publish_joint_state(
        PackedStringArray(["joint_1", "joint_2", "joint_3"]),
        PackedFloat64Array([pos_1, pos_2, pos_3]),
        PackedFloat64Array([vel_1, vel_2, vel_3])
    )
```

`joint_names`, `positions`, and `velocities` must be the same length and index-aligned.

### `JointJogController`

Publishes and subscribes to `control_msgs/msg/JointJog` messages on `/JointJog`, so a scene can both send jog commands out and receive them from an external ROS2 node.

```gdscript
var jog = JointJogController.new()

func _physics_process(delta):
    jog.spin_some()
    if jog.has_new_data():
        var names = jog.get_joint_names()
        var velocities = jog.get_joint_velocities()
        # apply the incoming velocities to your joints here

    # or command joints out to ROS2:
    jog.set_joint_velocity(
        PackedStringArray(["joint_1", "joint_2"]),
        PackedFloat64Array([0.5, -0.2])
    )
```

`has_new_data()` returns `true` once per received message and clears itself on read, so it's safe to poll every physics frame.

## Installation

### Prerequisites

- Godot 4.4
- ROS2 Jazzy Jalisco (installed to `/opt/ros/jazzy`, the default this build expects — see below for using a different distro/path)
- Jolt Physics Engine (for Godot)
- `control_msgs` and `sensor_msgs` (for the `JointJogController` / `JointStatePublisher` classes — these ship with a standard ROS2 install)

### Installing ROS2 Jazzy
A standard binary install of ROS2 Jazzy (`/opt/ros/jazzy`) is enough for this build — building ROS2 from source is not required. Follow the official instructions [here](https://docs.ros.org/en/jazzy/Installation.html). If you build from source, or install to a non-default location, update `ros_distro` and `ros_dir` in `SCsub` accordingly (see step 2 below).

### Download Godot 4.4 source code

1. Clone repository to a folder of your choice (more info [here](https://docs.godotengine.org/en/stable/contributing/development/compiling/getting_source.html)):
   ```bash
   git clone https://github.com/godotengine/godot.git

### Clone THIS repository (with the Mover6 submodule)

1. Clone repository, pulling in the `Godot_MOVER6_simulation_game` submodule at the same time:
   ```bash
   git clone --recurse-submodules https://github.com/jjhamer123/Godot-ROS-custom-build.git

   If you already have a plain clone without the submodule content, fetch it with:
   ```bash
   git submodule update --init --recursive

2. Edit the "SCsub" file in /path/to/Godot-ROS-custom-build/src/godot_custom_modules/godot_ros to match your ROS2 distribution and install directory, if different from the defaults. This build now defaults to:
   ```bash
   ros_distro = "jazzy"
   ros_dir = "/opt/ros/jazzy"

   `getIncludeDirs()` in the same file supports both a per-package `<pkg>/include/<pkg>` layout and a merged `include/<package>` layout, so it should find headers either way once `ros_dir` points at the right install.

3. Build Godot 4.x with the ROS2 module:
   ```bash
   cd /path/to/godot-source-code
   scons -j8 verbose=yes disable_exceptions=false SHOWBUILD=1 custom_modules=/path/to/Godot-ROS-custom-build/src/godot_custom_modules platform=linuxbsd

4. Build the Jolt Physics Engine add-on for Godot 4 ([details are here](https://github.com/godot-jolt/godot-jolt)):
   ```bash
   git clone https://github.com/godot-jolt/godot-jolt.git
   cd godot-jolt
   cmake --preset linux-gcc-x64
   cmake --build --preset linux-gcc-x64-distribution
   cmake --build --preset linux-gcc-x64-editor-distribution
   cmake --install build/linux-gcc-x64 --config Distribution --prefix /path/to/Godot-ROS-custom-build/src/Godot_MOVER6_simulation_game
   cmake --install build/linux-gcc-x64 --config EditorDistribution --prefix /path/to/Godot-ROS-custom-build/src/Godot_MOVER6_simulation_game

   Jolt Physics is found in the "addons/godot-jolt" directory. Edit "godot-jolt.gdextension" and set "**compatibility_maximum = "4.4"**".

## Usage

### Running an arm simulation/digital shadow (Mover6 example)
1. Source ROS2:
   ```bash
   source /opt/ros/jazzy/setup.bash
2. Run Godot in verbose/debug_info mode:
   ```bash
   ./godot.linuxbsd.editor.x86_64 -v -d
3. Open the `Godot_MOVER6_simulation_game` submodule folder as the project and hit play to run `Robot.tscn`. From there, `ros_2_arm.gd` (`RosArmBridge`) spins up a `JointJogController` and `JointStatePublisher` and starts publishing/consuming ROS2 joint data — toggle its `mode` export between **Simulation** and **Shadow** depending on whether you want incoming `/JointJog` commands to actually move the arm, or just want it to report its current physical state. Swap in a different URDF-imported arm submodule with the same `RobotJointController` interface to point this at another robot.

### Using the joint-level ROS2 interface directly
`JointStatePublisher` and `JointJogController` don't need any extra setup beyond the module being built in — attach a script to any node in your scene, instantiate the class, and call `spin_some()` each physics frame as shown in [New ROS2 classes](#new-ros2-classes) above.

## License
This project is licensed under the GNU General Public License v3.0.
