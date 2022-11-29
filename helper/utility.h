#pragma once

/////////////////////////////////////////////////////////////////////////////
// Bytes
std::vector<int> StringToBytes(const std::string& svInput, bool bNullTerminator);
std::vector<int> PatternToBytes(const std::string& svInput);
