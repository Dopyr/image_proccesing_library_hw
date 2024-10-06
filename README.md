# Image Processing Project
This project implements fundamental steps of Digital Image Processing as part of a term project. The application allows users to perform various preprocessing, filtering, and morphological operations on images. It has been developed in [insert programming language here], and users can open an image, process it, and save the result in various formats such as .jpg, .bmp, and .png.

## Features
### 1. Image Loading and Saving
Users can select an image from their computer.
After processing, the result can be saved in a chosen format: .jpg, .bmp, or .png.
### 2. Preprocessing Menu 1
Convert a color image to grayscale.
Convert a grayscale image to black and white (threshold-based).
Zoom in and out.
Crop a specific region from the image.
### 3. Preprocessing Menu 2
Generate a histogram.
Perform histogram equalization (on grayscale images).
Quantize the image (with a selected number of tones).
### 4. Filtering Menu
Apply Gaussian blur (customizable with a standard deviation (σ) value).
Apply sharpening filter.
Edge detection filter.
Mean filter.
Median filter.
Contra Harmonic Mean filter.
### 5. Morphological Operations
Dilation on black-and-white images.
Erosion on black-and-white images.
Skeletonization of black-and-white images.
Usage Instructions
Select Image: The user selects an image to load from their computer.
Preprocessing: Choose a preprocessing operation from either Menu 1 or Menu 2.
Filtering: Apply filters to the preprocessed image.
Morphological Operations: Perform desired morphological operations on the filtered image.
Save Image: Save the final image in the desired format.
Project Structure
The project contains the source code folder, along with a info.txt file describing the development environment and compiler version used.
All algorithms used in the project have been implemented from scratch without using any pre-built image processing libraries.

## Demo
The project is structured to ensure that each processing step builds on the results of the previous one. Users will be able to see the effect of applying different filters and morphological operations to images step by step.
