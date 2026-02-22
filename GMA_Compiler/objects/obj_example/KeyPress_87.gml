if (!ProcIdExists(procId)) {
  str = "No window identifiers to iterate.";
  ShowMessage(str);
  exit;
}
info = ProcInfoFromProcIdEx(procId, KINFO_OWID);
widsize = OwnedWindowIdLength(info);
if (widsize) {
  for (jj = 0; jj < widsize; jj += 1) {
    str = "Continue window identifier iteration?" + chr(13) + chr(10) + chr(13) + chr(10) + OwnedWindowId(info, jj);
    q = ShowQuestion(str);
    if (!q) { FreeProcInfo(info); exit; } 
  }
} else {
  str = "No window identifiers to iterate.";
  ShowMessage(str);
}
FreeProcInfo(info);