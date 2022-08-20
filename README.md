# OCR A-Level Computer Science Project - NES Emulator

This is an NES Emulator created for the OCR A-Level programming project.

Because of the amount of changes made (including the topic),
the Git history is pretty scuffed. The repository serves more to just keep it all together.

Building:

Install SFML.
Clone imgui and imgui-sfml from GitHub to the libs folder i.e. libs/imgui libs/imgui-sfml

```
cd core/build
cmake ..
cd ../../
cd frontend/build
cmake ..
cd ../..
make release
