if (!ProcIdExists(procId)) {
  str = "No environment to iterate.";
  ShowMessage(str);
  exit;
}
info = ProcInfoFromProcIdEx(procId, KINFO_ENVV);
envsize = EnvironmentLength(info);
if (envsize) {
  for (jj = 0; jj < envsize; jj += 1) {
    str = "Continue environment iteration?" + chr(13) + chr(10) + chr(13) + chr(10) + Environment(info, jj);
    q = ShowQuestion(str);
    if (!q) { FreeProcInfo(info); exit; } 
  }
} else {
  str = "No environment to iterate.";
  ShowMessage(str);
}
FreeProcInfo(info);
