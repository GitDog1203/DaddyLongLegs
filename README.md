# DaddyLongLegs

I recreated *Daddy Long Legs*, originally made by Set Snail. I plan to add more features, but for now I think it looks great!

I used Gemini to help with parts of the project, but I did most of the work myself.

Here's everything I did! It could be better, but this is what I have for now.

## Quick Start (Precompiled Firmware)

### Prerequisites (Windows)

Before installing the game, you need Python and esptool.

**1. Install Python**
- Download Python from [python.org/downloads](https://www.python.org/downloads/).
- During installation, check **Add Python to PATH**.
- Open Command Prompt and verify the installation:

```cmd
python --version
```

**2. Install esptool**

In Command Prompt, run:

```cmd
python -m pip install esptool
```

Verify it is installed:

```cmd
python -m esptool version
```

**3. Connect your Cardputer ADV**
- Connect your Cardputer ADV to your computer using USB.
- Find its COM port in Windows Device Manager under **Ports (COM & LPT)**.

### Installing the Game!

1. Go to the [Releases page](https://github.com/GitDog1203/DaddyLongLegs/releases).
2. Download `DaddyLongLegs.ino.merged.bin` from the latest release.
3. Open Command Prompt.

**Before continuing:** Replace `YOUR_COM_PORT` in the commands below with your Cardputer's actual COM port, such as `COM5`, `COM4`, or `COM12`. Do not type `YOUR_COM_PORT` literally.

4. Run this command to check your connection:

```cmd
python -m esptool --port YOUR_COM_PORT chip-id
```

5. Flash the game using this command:

```cmd
python -m esptool --chip esp32s3 --port YOUR_COM_PORT --baud 460800 write-flash --flash-size 8MB 0x0 "PATH_TO_FIRMWARE.bin"
```

Replace `PATH_TO_FIRMWARE.bin` with the full path to your downloaded `.bin` file. Keep the quotation marks around the path.

6. Wait for flashing to finish. The Cardputer will restart.
7. Enjoy the game!

**Important:** Flashing this firmware erases the existing firmware and data stored on your Cardputer ADV.

## Controls

- **W:** Main game button

Have fun! (=

Here's a Video: https://youtu.be/QfPZ_8-vjX0?si=ZY2NQLE3qIx7xw1Y

![DaddyLongLegs gameplay screenshot](https://github.com/user-attachments/assets/82103df1-cc5b-4511-9518-33f11c519e91)

**Note:** `StepGame.ino` was replaced with `DaddyLongLegs.ino`. They are the same project; the file was renamed!

## Credits

- Original *Daddy Long Legs* game: Set Snail
- Hardware: M5Stack Cardputer ADV
- AI assistance: Gemini
