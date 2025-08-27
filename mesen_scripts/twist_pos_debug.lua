function printInfo()
  --Get the emulation state
  state = emu.getState()
  
  --Get the mouse's state (x, y, left, right, middle)
  mouseState = emu.getMouseState()  
  
  --Select colors based on whether the left button is held down

    bgColor = 0x302060FF
    fgColor = 0x30FF4040
  
 
  kris_tx = (emu.readWord(emu.getLabelAddress("_twisted").address+1, emu.memType.nesMemory, false) >> 8)
  kris_ty = (emu.readWord(emu.getLabelAddress("_twisted").address+3, emu.memType.nesMemory, false) >> 8)

  emu.drawRectangle(kris_tx-4, kris_ty, 16, 16, fgColor, true, 1)
  emu.displayMessage("Script", kris_ty)

end

--Register some code (printInfo function) that will be run at the end of each frame
emu.addEventCallback(printInfo, emu.eventType.endFrame);

--Display a startup message
emu.displayMessage("Script", "Example Lua script loaded.")
