function printInfo()
  --Get the emulation state
  state = emu.getState()
  
  --Get the mouse's state (x, y, left, right, middle)
  mouseState = emu.getMouseState()  
  
  --Select colors based on whether the left button is held down

    bgColor = 0x302060FF
    fgColor = 0x30FF4040
  
 
  twisted_x = (emu.readWord(emu.getLabelAddress("_twisted").address+1, emu.memType.nesMemory, false) >> 8)
  twisted_y = (emu.readWord(emu.getLabelAddress("_twisted").address+3, emu.memType.nesMemory, false) >> 8)

  emu.drawRectangle(twisted_x-4, twisted_y, 16, 16, fgColor, true, 1)

  emu.drawString(12, 8, "Twisted State: " .. emu.readWord(emu.getLabelAddress("_twisted").address+19, emu.memType.nesMemory, false), 0xFFFFFF, 0xFF000000)
  emu.drawString(12, 16, "Twisted StateTimer: " .. emu.read(emu.getLabelAddress("_twisted").address+23, emu.memType.nesMemory, false), 0xFFFFFF, 0xFF000000)
  emu.drawString(12, 24, "Twisted LookDir: " .. emu.read(emu.getLabelAddress("_twisted").address+26, emu.memType.nesMemory, false), 0xFFFFFF, 0xFF000000)


end

--Register some code (printInfo function) that will be run at the end of each frame
emu.addEventCallback(printInfo, emu.eventType.endFrame);

--Display a startup message
emu.displayMessage("Script", "Example Lua script loaded.")
