# GPL Interpreter Binary Files

Download the appropriate binary file for your machine.
If you are a Windows user using Ubuntu bash, you must download one of the binary files for Linux OS.

The files you download are not executable, so you need to convert (change mode) to make them executable.

For example, let's assume you downloaded `Linux/gpl_ubuntu_64bit_x86`.

Grant execute permission:
```
$chmod 700 gpl_ubuntu_64bit_x86
```

Test your executable:
```
$./gpl_ubuntu_64bit_x86 Samples/rectangle.gpl
```

If you receive an error, here are a few things you could try:
1. Check your OS and architecture, then download the correct binary file.
2. Come and talk to me.
