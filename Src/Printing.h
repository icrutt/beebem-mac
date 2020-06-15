//**CARBON**void MyDrawIntoPDFPage(CGContextRef pdfContext, PMRect pageRect, int starty, int nlines);
//**CARBON**static OSStatus MyCreatePageFormat(PMPrintSession printSession, PMPageFormat *pageFormat);
//**CARBON**static OSStatus DoPageSetupDialog(PMPrintSession printSession, PMPageFormat* pageFormat, Handle* flattendedPageFormat);
//**CARBON**static OSStatus	DetermineNumberOfPagesInDoc(PMPageFormat pageFormat, UInt32* numPages);
//**CARBON**static OSStatus DoPrintLoop(PMPrintSession printSession, PMPageFormat pageFormat, PMPrintSettings printSettings, int starty, int nlines);
//**CARBON**static OSStatus DoPrintDialog(PMPrintSession printSession, PMPageFormat pageFormat, PMPrintSettings* printSettings);
void ProcessPrintCommand(int starty, int nlines);
void DoPageSetup(void);

//**CARBON**extern PMPageFormat pageFormat;
//**CARBON**extern PMPrintSettings printSettings;
//**CARBON**extern Handle flattenedPageFormat;
