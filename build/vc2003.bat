rem set VC71_BOOST_DIR=
rem set VC71_ICU_DIR=
rem set VC71_WXWIN=
rem set VC71_XPRESSIVE_INC=
rem set VC71_AUI_INC=
rem set VC71_AUI_LIB=
bakefile -D BOOST_DIR="%VC71_BOOST_DIR%" -D ICU_DIR="%VC71_ICU_DIR%" -D WXWIN="%VC71_WXWIN%" -D XPRESSIVE_INC="%VC71_XPRESSIVE_INC%" -D AUI_INC="%VC71_AUI_INC%" -D AUI_LIB="%VC71_AUI_LIB%" -f msvs2003prj wxmedit.bkl -o ..\vc2003\wxmedit
