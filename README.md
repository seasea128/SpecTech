
# SpecTech

SpecTech is a visualisation application that utilises Virtual Reality for observing and interacting with three-dimentional models.


## Features

* Modify features of existing models such as:
	- Colour (<span style="color:red">R</span><span style="color:green">G</span><span style="color:blue">B</span>)
	- Name
	- Visibility
	- Metallic
	- Roughness
	- Anisotropy
	- Anisotropy Rotation
* Colour can be altered using sliders or a colour picker.
* Edit or delete individual model components.
* Open models individually or as a collective.
* Open multiple models at once.
* Manually rotate models freely about xyz axes.
* Includes filters:
	- Shrink filter: shrinks each triangle inside the model.
	- Clip filter: cuts off model from a specfied point and direction.
	- Filters can be removed.
* Observe models in Virtual Reality - OpenVR.
* Edit the rotational speed of the model in VR.
* **Light** and **dark** modes are available upon button click for Doxygen.


## Requirements

The following requirements only apply if compiling from source.

* CMake: 3.5
* Qt: 6.6.1
* VTK: 9.3.0

>[Installers](#Install) are available.


## Install

An `exe` installer is available for Windows.

## Repo tree

```
├── .github/workflows/                
|   └── doxygen-deploy.yml       # Passes the sources and generates documentation
|             
├── doxygen-awesome-css/         # Git Submodule: Doxygen Awesome (by jothepro)
|  
└── source/       
    ├── RenderThread/            # Handles VR content
    ├── icons/                   # Stores icons
    ├── skybox/                  # Displaying the environment
    ├── vrbindings/              # Allows the VR controllers to interact with objects
    └── CMakeLists.txt           # Main CMakeLists
```

## Documentation
The Doxygen formatted comments are hosted [here] (https://seasea128.github.io/2023_GROUP_1/)

The Doxygen theme is available [here](https://jothepro.github.io/doxygen-awesome-css/index.html).  by [jothepro](https://github.com/jothepro).