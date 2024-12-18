# SignalPlot

![nn](https://ifh.cc/g/B0hfAB.gif)

The **SignalPlot** project provides a comprehensive tool for real-time signal analysis and visualization. It combines efficient data acquisition, processing, and graphical representation, allowing users to gain deeper insights into their signals.

## Overview

SignalPlot is designed to visualize and process signal data in both time and frequency domains. The application employs Fast Fourier Transform (FFT) and Inverse FFT (IFFT) algorithms to analyze and reconstruct signals while offering an intuitive interface for customization and interaction.

Key objectives include:

- **Real-Time Visualization**: Plot and update time-domain and frequency-domain signals dynamically.
- **Signal Processing**: Apply FFT/IFFT transformations, utilize Hanning or Hamming windows, and evaluate signal properties.
- **Serial Communication**: Acquire signals from external devices via serial port connections.
- **Graph Customization**: Modify graph parameters, axes, and styles interactively.
- **User Interaction**: Enable features like tooltips, data scaling, and user-driven parameter adjustment.

## Features

### Signal Visualization
- Graphs for **time-domain signals**, **frequency-domain (FFT)**, and **reconstructed IFFT signals**.
- Dynamic updates for real-time signal monitoring.
- Configurable axes, labels, and color schemes.

### Signal Processing
- Efficient FFT and IFFT implementations for accurate analysis and reconstruction.
- Support for windowing techniques (Hanning and Hamming) to minimize spectral leakage.
- Automatic computation of frequency and magnitude ranges.

### Serial Communication
- Seamlessly connects to external devices using serial ports.
- Lists available serial ports for user selection.
- Allows baud rate and device configuration for reliable data transfer.

### User Interaction
- Intuitive GUI with status messages and interactive controls.
- Slider-based adjustments for scaling and visualization preferences.
- Tooltip display for precise data inspection.

## Purpose

SignalPlot aims to assist engineers, researchers, and enthusiasts in signal analysis tasks by offering an integrated platform for:

- Real-time data acquisition and visualization.
- Frequency and time-domain signal exploration.
- Signal processing with customizable parameters.
- Experimentation and debugging of signal systems.

This tool is particularly useful for scenarios such as:
- Evaluating the performance of sensors and electronic systems.
- Conducting educational demonstrations of signal processing concepts.
- Developing prototypes requiring rapid signal feedback.

## Technical Highlights

- **Graphing Library**: Utilizes `QCustomPlot` for high-quality and interactive plotting.
- **FFT Implementation**: Recursive Cooley-Tukey FFT algorithm for efficient spectral analysis.
- **IFFT Support**: Enables reconstruction of signals with high fidelity.
- **Dynamic Updates**: Supports real-time interaction and responsiveness.
- **Serial Port Integration**: Communicates effectively with external hardware to acquire live data streams.

## Contributions

Contributions to this repository are welcome! Whether you wish to improve signal processing algorithms, enhance UI/UX features, or add compatibility with new devices, feel free to open an issue or submit a pull request.

---

For detailed instructions on setup and usage, refer to the documentation or contact the repository maintainer. Let SignalPlot assist in making your signal analysis tasks seamless and insightful.

