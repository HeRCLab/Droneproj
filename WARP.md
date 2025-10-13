# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

Project-specific rules and workflows for the ArduPilot codebase in this directory.

Sections
- Commands you will commonly use (build, SITL, tests, linting)
- High-level architecture overview to orient changes quickly

Common commands

- Install prerequisites (Linux)
  - These scripts set up compilers, libraries, and Python tooling used throughout the repo.
  ```bash path=null start=null
  Tools/environment_install/install-prereqs-ubuntu.sh -y
  # Re-open your shell to pick up any environment changes
  ```

- Configure and build (SITL and hardware boards)
  - Always invoke waf from the repository root (see BUILD.md). Do not use sudo.
  ```bash path=null start=null
  # Configure for SITL (software-in-the-loop), with debug symbols
  ./waf configure --board sitl --debug

  # Build a vehicle group (e.g., Copter)
  ./waf copter

  # Build for a hardware board (example: CubeBlack)
  ./waf configure --board CubeBlack
  ./waf copter

  # List supported boards
  ./waf list_boards

  # Show all targets (vehicles, tools, tests, examples)
  ./waf list
  ```

- Run SITL quickly via sim_vehicle.py
  - The repo provides a convenient launcher for SITL under Tools/autotest.
  ```bash path=null start=null
  # ArduCopter SITL, quad frame, with GCS map and MAVProxy console
  Tools/autotest/sim_vehicle.py -v ArduCopter -f quad --map --console

  # ArduPlane SITL
  Tools/autotest/sim_vehicle.py -v ArduPlane --map --console

  # Clean/wipe parameters for a fresh run
  Tools/autotest/sim_vehicle.py -v ArduCopter -f quad -w --map --console
  ```

- Upload to target hardware (when supported)
  ```bash path=null start=null
  # Configure rsync destination for Linux targets
  ./waf configure --board <board> --rsync-dest <user@host:/>

  # Upload a specific binary produced by the build
  ./waf --targets bin/arducopter --upload
  ```

- Unit tests and test selection
  - Waf integrates unit tests and can run “relevant” tests incrementally (BUILD.md).
  ```bash path=null start=null
  # Build everything and run relevant tests
  ./waf check

  # Run all tests regardless of prior state
  ./waf check --alltests
  # or
  ./waf check-all

  # Build a single C++ unit test target (example from BUILD.md)
  ./waf --targets tests/test_math
  ```

  - Python tests (pytest) exist in specific tool subtrees (e.g., Tools/ros2/ardupilot_dds_tests). To run a single test:
  ```bash path=null start=null
  # Example: run one ROS2 DDS test (requires ROS2 environment and dependencies)
  python3 -m pytest Tools/ros2/ardupilot_dds_tests/test -k test_battery_msg_received
  ```

- Pre-commit and linting
  - The repository uses pre-commit to enforce formatting and static checks on selected paths (see .pre-commit-config.yaml and pyproject.toml).
  ```bash path=null start=null
  # Install hooks locally
  pre-commit install

  # Run all hooks across the repo (useful before committing)
  pre-commit run --all-files
  ```
  - Notable configured hooks include: mixed line endings, basic YAML/XML checks, codespell; plus targeted Python formatting/quality (black, flake8) for Tools/ros2 and AP_DDS Python.

High-level architecture

- Build system (waf)
  - Top-level ./waf is a wrapper around the waf submodule (modules/waf). Build “groups” organize outputs into build/<board>/<group>/, e.g., bin (vehicles), tests (unit tests), examples, benchmarks. Key shortcuts: ./waf copter | plane | rover | sub | antennatracker. See BUILD.md for program groups and check semantics.

- Vehicles (top-level directories)
  - ArduCopter/, ArduPlane/, Rover/, ArduSub/, AntennaTracker/ contain the vehicle-specific flight applications. Each defines a wscript that registers its main binary/binaries and ties into the shared libraries tree.

- Shared libraries (libraries/)
  - Core flight subsystems (AHRS/EKF, sensors, mission, navigation, control, IO, logging, scripting, etc.). Sub-libraries often contain examples/, tests/, and a wscript for their own build targets. SITL-specific shims live under libraries/AP_HAL_SITL and libraries/SITL.

- Hardware Abstraction Layer (HAL)
  - Multiple HAL backends exist (AP_HAL_ChibiOS for embedded STM32 class boards, AP_HAL_ESP32 for ESP32, AP_HAL_QURT for Snapdragon, AP_HAL_SITL for simulation). The selected --board during configure determines which HAL is compiled and how drivers are wired.

- Tools/
  - Development tooling and components not part of the main flight binaries:
    - autotest/: the simulation and test harness (sim_vehicle.py launcher, test suites, mission scripts)
    - ardupilotwaf/: ArduPilot-specific waf extensions (boards, toolchains, gtest/gbenchmark integration, summaries)
    - scripts/: CI helpers, formatting, size tracking, coverage, parameter metadata, etc.
    - AP_Periph/ and AP_Bootloader/: companion firmware targets with their own build entries

- Modules/ and third-party
  - Submodules such as waf, mavlink/pymavlink, gtest, gbenchmark, ChibiOS, lwIP, and others are vendored in modules/ or under libraries’ subtrees where appropriate.

- Continuous Integration (GitHub Actions)
  - Workflows cover SITL for each vehicle, ChibiOS builds, unit tests, coverage, and pre-commit. The unit test workflow drives Tools/scripts/build_ci.sh with matrix toolchains (GCC/Clang) and configs (unit-tests/examples/sitl), mirroring the local commands above.

Notes and tips specific to this repo
- Invoke waf from the repository root; avoid sudo with waf.
- If submodules drift (e.g., switching branches), use:
  ```bash path=null start=null
  ./waf submodulesync
  # If needed as a last resort
  ./waf submodule_force_clean
  ```
- Output paths: after building, binaries are under build/<board>/bin/, e.g., build/sitl/bin/arducopter for SITL.

References
- BUILD.md and README.md in the repo root document the authoritative build, groups, and test behaviors.
- .pre-commit-config.yaml and pyproject.toml define the active lint/format rules and their scope.
