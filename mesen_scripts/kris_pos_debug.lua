function printInfo()
  --Get the emulation state
  state = emu.getState()
  
  --Get the mouse's state (x, y, left, right, middle)
  mouseState = emu.getMouseState()  
  
  --Select colors based on whether the left button is held down

    bgColor = 0x302060FF
    fgColor = 0x30FF4040
  
  
  --Draw some rectangles and print some text
  emu.drawRectangle(8, 8, 128, 32, bgColor, true, 1)
  emu.drawRectangle(8, 8, 128, 32, fgColor, false, 1)
  
  kris_tx = (emu.read(emu.getLabelAddress("_kris").address, emu.memType.nesMemory, false) + 7 >> 4)
  kris_ty = (emu.read(emu.getLabelAddress("_kris").address+1, emu.memType.nesMemory, false) + 7 >> 4)
  
  emu.drawRectangle(kris_tx*16, kris_ty*16, 16, 16, fgColor, true, 1)
  
  emu.drawString(12, 8, "Kris x: " .. emu.read(emu.getLabelAddress("_kris").address, emu.memType.nesMemory, false), 0xFFFFFF, 0xFF000000)
  emu.drawString(12, 16, "Kris y: " .. emu.read(emu.getLabelAddress("_kris").address+1, emu.memType.nesMemory, false), 0xFFFFFF, 0xFF000000)
  emu.drawString(12, 24, "Kris tx: " .. kris_tx, 0xFFFFFF, 0xFF000000)
  emu.drawString(12, 32, "Kris ty: " .. kris_ty, 0xFFFFFF, 0xFF000000)

end

--Register some code (printInfo function) that will be run at the end of each frame
emu.addEventCallback(printInfo, emu.eventType.endFrame);

--Display a startup message
emu.displayMessage("Script", "Example Lua script loaded.")