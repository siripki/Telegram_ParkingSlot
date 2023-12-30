void spiffSetup() {
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
}

String readFile(const char *path) {
  File file = SPIFFS.open(path, "r");
  if (!file) {
    Serial.println("Failed to open file for reading "+String(path));
    return String();
  }
  String fileContent;
  while (file.available()) {
    fileContent += (char)file.read();
  }
  return fileContent;
}

void writeFile(const char *path, const char *fileContent) {
  File file = SPIFFS.open(path, "w");
  if (file.print(fileContent)) {
    Serial.println("- file written "+String(path));
  } else {
    Serial.println("- write failed "+String(path));
  }
}

void readData(){
  String data;
  data = readFile("/cardStatus.txt");
  if (data == ""){
    dataDefault();
    data = readFile("/cardStatus.txt");
    decodeJson(data);
  }
  else{
    decodeJson(data);
  }
}

void dataDefault() {
  for (int i = 0; i < cardTotal; i++) {
    cards[i].status = 0;
  }
  
  String data;
  codeJson(data);
  writeFile("/cardStatus.txt", data.c_str());
}