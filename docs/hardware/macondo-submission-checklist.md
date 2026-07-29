# Macondo submission checklist

This is the checklist I use before submitting Blip to Macondo. It separates the
repository work from the parts that need my own hands, photos, and decisions.

## Repository package

- [x] `PCB/` contains the authoritative KiCad project: schematic, board, and project file.
- [x] `BOM/Hackpad_final_BOM.csv` is valid CSV and matches the populated PCB references and footprint counts.
- [x] `src/firmware/` contains the PlatformIO firmware project.
- [x] `src/` contains the simulator and the shared device-event boundary.
- [x] `CAD/Blip-PCB-Assembly.step` is included as mechanical evidence.
- [x] `docs/hardware/` contains the pin-map contract, hardware-change guide, validation reports, and research notes.
- [x] The root README explains what Blip is, how the repository is organized, how to run the simulator, and what the current hardware status means.
- [x] The repository checks pass locally.

Run these from the repository root:

```sh
sh scripts/verify-hardware-source-of-truth.sh
sh scripts/verify-minimal-pcb-corrections.sh
sh scripts/verify-bom-against-pcb.sh
sh scripts/verify-hardware-validation-gate.sh
npm test
```

## Before I submit the hardware design for review

- [ ] Commit the final README, BOM, and verification-script changes.
- [ ] Push the commit to `main` and open the GitHub repository link in a private browser window.
- [ ] Open the KiCad project from a clean clone and confirm the schematic and PCB load without missing libraries.
- [ ] Open the current STEP export and check the board outline, mounting holes, USB-C access, display clearance, and encoder clearance.
- [ ] Confirm that the listed parts and prices still match the parts I intend to order.
- [ ] Make sure the Macondo project title and description describe the current two-display design, not an older one-screen version.

## Before I call the hardware project shipped

- [ ] Order or otherwise obtain the parts.
- [ ] Assemble the board or first article and take clear photos of the real hardware.
- [ ] Test the USB-C connection, matrix, both encoders, both displays, and the controller.
- [ ] Add honest journal entries on the Macondo project page with my own notes, photos, and what went wrong or changed.
- [ ] Complete and sign the final manufacturing checklist in `hardware-change-guide.md`.

The automated checks prove that the repository is internally consistent. They do
not prove that a real part fits, that the board has been manufactured, or that a
physical control feels right. Those are my checks to complete and document.
