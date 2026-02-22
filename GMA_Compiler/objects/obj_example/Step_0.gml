if (pid != 0 && CompletionStatusFromExecutedProcess(pid)) {
  output = ExecutedProcessReadFromStandardOutput(pid);
  if (string_replace_all(string_replace_all(output, "\n", ""), "\r", "") != "") {
    ShowMessage("Process Output:\n\n" + output);
  }
  FreeExecutedProcessStandardOutput(pid);
  FreeExecutedProcessStandardInput(pid);
  pid = 0;
}