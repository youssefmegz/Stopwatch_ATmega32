# Stopwatch_ATmega32

This project is a **Timer System** built using an **ATmega32 microcontroller**. It operates in two modes: **Up Timer** (counting up from zero) and **Down Timer** (counting down from a preset time**).
The system offers full user control through push buttons, provides visual and audio feedback, and handles all corner cases for reliable operation.

## Project Overview

### Timer Modes

- **Up Timer Mode**: Default mode where the timer starts at zero and counts up.
- **Down Timer Mode**: The timer counts down from a user-defined preset time.

### Key Functions

- **Control Options**:
  - **Reset**: Resets the timer to zero or the preset value (in Down Timer mode).
  - **Pause**: Temporarily pauses the timer.
  - **Resume**: Resumes counting from the paused state.
  
- **Time Adjustment**:
  - Easily adjust the **hours**, **minutes**, and **seconds** through push buttons.

- **Feedback System**:
  - **Visual Feedback**: Using six multiplexed seven-segment displays to show the current time.
  - **Audio Feedback**: A buzzer is activated when the timer reaches zero in Down Timer mode.
  - **LED Indicator**: An LED provides a visual signal when the alarm is triggered.

- **Corner Cases Considered**:
  - Handles edge cases like zero time setting, negative counts, and button debouncing for smooth operation.

## Hardware Components

- **ATmega32 Microcontroller**: The core of the system responsible for processing user input and controlling the timer logic.
- **Six Multiplexed Seven-Segment Displays**: Used to display the timer's current time.
- **Push Buttons**: For user interaction (time adjustments, start, pause, reset).
- **Buzzer**: Provides an alarm sound when the timer reaches zero in Down Timer mode.
- **LED Indicators**: Activated along with the buzzer to provide a visual alert.



