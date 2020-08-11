#ifndef CONST_HTML_H
#define CONST_HTML_H

#include <QString>

const QString c_strTemplate = "<html><head/><body><p align=\"center\"><span style=\" color:%2;\">%1</span></p></body></html>";

const QString c_strChatMsgTemplate = "<head>"
                                     "<meta name=\"qrichtext\" content=\"1\" />"
                                     "<style type=\"text/css\">"
                                     "p, li { white-space: pre-wrap; }"
                                     "</style>"
                                     "</head>"
                                     "<body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">"
                                     "%1"
                                     "</body>";

const QString c_strMsgTemplate = "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                 "<span style=\" font-size:8.25pt; color:#55557f;\">%1</span>"
                                 "<span style=\" font-size:8.25pt;\"></span>"
                                 "<span style=\" font-size:8.25pt; color:%3;\"> %4</span>"
                                 "<span style=\" font-size:8.25pt;\"> : %2</span></p>";

const QString c_strSystemTag = "System";
const QString c_strOwnTag = "Me";

#endif // CONST_HTML_H
