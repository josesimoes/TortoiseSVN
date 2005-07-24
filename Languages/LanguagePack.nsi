; Script generated by the HM NIS Edit Script Wizard.
; HM NIS Edit Wizard helper defines
!define PRODUCT_PUBLISHER "TortoiseSVN"
!define PRODUCT_VERSION "1.2.1"
!define PRODUCT_WEB_SITE "http://tortoisesvn.tigris.org/"
!define MUI_WELCOMEPAGE_TITLE_3LINES true

!include "PRODUCT.nsh"

;Makro to upgrade existing language dlls without registering them
!define UPGRADEDLL_NOREGISTER
!include "UpgradeDLL.nsh"

SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Language files
!insertmacro MUI_LANGUAGE ${InstLang}

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\bin\LanguagePack_${PRODUCT_VERSION}_${CountryCode}.exe"
ShowInstDetails show

Section "Hauptgruppe" SEC01
  ReadRegStr $0 HKLM "software\TortoiseSVN" "Directory"
  StrCmp $0 "" NotInstalled Installed
Installed:
  Goto Proceed
NotInstalled:
  MessageBox MB_OK "TortoiseSVN not installed!"
  Abort
Proceed:
!ifdef LangHelp
  StrCpy $INSTDIR "$0\bin"
  SetOutPath "$INSTDIR"
  File "..\doc\output\TortoiseSVN_${CountryCode}.chm"
!endif
  StrCpy $INSTDIR "$0\Languages"
  SetOutPath "$INSTDIR"
  File /nonfatal "..\..\Common\Spell_Thes\${CountryCode}*.*"
  File /nonfatal "..\..\Common\Spell_Thes\th_${CountryCode}*.*"
  !insertmacro UpgradeDLL "..\bin\TortoiseProc${CountryID}.dll" "$INSTDIR\TortoiseProc${CountryID}.dll" "$INSTDIR"
  !insertmacro UpgradeDLL "..\bin\TortoiseMerge${CountryID}.dll" "$INSTDIR\TortoiseMerge${CountryID}.dll" "$INSTDIR"
!ifdef MoFile
  StrCpy $INSTDIR "$0\Languages\${CountryCode}\LC_MESSAGES"
  SetOutPath "$INSTDIR"
  File "subversion.mo"
!endif
SectionEnd

Section -Post
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_FUNCTION_DESCRIPTION_END
