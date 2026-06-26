> [!IMPORTANT]
> The core features of FlashPoint are still being worked on and are not yet stable/usable

> **This is a personal fork of [CrossInk](https://github.com/uxjulia/crossink)** with added support for reviewing flashcards and syncing with Anki

## Why does this fork exist

I wanted to be able to sync the Anki flashcards on my laptop with my XTeink X4 and review them on the go (without being distracted by my phone).
No Crosspoint forks support syncing with Anki, and those which do support local flashcards use outdated spaced repetition algorithms such as SM-2.
It is this reason that I am aiming for FlashPoint to both support two-way syncing with Anki and the newer FSRS v5 spaced repetition algorithm.

### Why did I fork CrossInk and not CrossPoint reader

After testing both [CrossPoint](https://github.com/crosspoint-reader/crosspoint-reader) and its fork [CrossInk](https://github.com/uxjulia/crossink), I found CrossInk nicer to use due to the minimal statistics and many nice-to-haves it adds on.
Overall, they are not too different though so FlashPoint will still feel very familiar if you are coming from CrossPoint Reader.

### Progress

- [ ] Basic plain text card reviewing, scheduling, saving, and syncing
- [ ] Basic card management (bury/suspend/flag)
- [ ] Deck screen
- [ ] Settings (fonts etc...)
- [ ] Cards with images
- [ ] Other types of cards (cloze, etc...)

### Development Device Simulator

The [device simulator](https://github.com/uxjulia/crosspoint-simulator) renders the e-ink display in an SDL2 window so firmware changes can be sanity-checked without flashing hardware.

See [Simulator](./docs/simulator.md) for setup, platform notes, keyboard controls, and cache tips.

---
## Installation

See [Installation](./docs/installation.md) for step-by-step flashing and revert instructions.

---

## Documentation

- [User Guide](./USER_GUIDE.md)
- [Installation](./docs/installation.md)
- [Font Build Variants](./docs/font-build-variants.md)
- [Reader Features](./docs/reader-features.md)
- [Controls](./docs/controls.md)
- [Simulator](./docs/simulator.md)
- [Data Cache](./docs/data-cache.md)
- [Web server usage](./docs/webserver.md)
- [Web server endpoints](./docs/webserver-endpoints.md)
- [Common issues](./docs/troubleshooting.md)
- [Project scope](./SCOPE.md)
- [Contributing docs](./docs/contributing/README.md)

---

## Development quick start

FlashPoint uses PlatformIO for building and flashing firmware.

See [Getting Started](./docs/contributing/getting-started.md) for prerequisites, clone setup, hooks, and validation commands.

### Build / flash / monitor

Connect your Xteink X4 or X3 via USB-C and run:

```sh
pio run -e tiny --target upload
```

Replace `tiny` with another build variant if needed. See [Font Build Variants](./docs/font-build-variants.md).

See [Testing and Debugging](./docs/contributing/testing-debugging.md) for serial logging, simulator checks, static analysis, and bug-report guidance.

---

## Internals

The ESP32-C3 has about 380 KB of usable RAM, so FlashPoint stores reusable book, flashcard, and device data on the SD card instead of rebuilding everything in memory.

See [Data Cache](./docs/data-cache.md) for the `.crosspoint` layout and [File Formats](./docs/file-formats.md) for binary cache details.
