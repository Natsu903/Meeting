#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QCompleter>
#include <QStringList>
#include <QPair>
#include <QVector>

class Completer: public QCompleter
{
Q_OBJECT
public:
    explicit Completer(QWidget *parent= nullptr);
};

// 自定义文本编辑器类，支持@功能和IP地址自动补全
class MyTextEdit : public QWidget
{
    Q_OBJECT
private:
    QPlainTextEdit *edit;   // 文本编辑器控件
    Completer *completer;   // 自动补全器
    QVector<QPair<int, int> > ipspan;   // 存储已选择IP地址在文本中的位置范围

public:
    explicit MyTextEdit(QWidget *parent = nullptr);
    // 获取纯文本内容
    QString toPlainText();
    // 设置纯文本内容
    void setPlainText(QString);
    // 设置占位符文本
    void setPlaceholderText(QString);
    // 设置占位符文本
    void setCompleter(QStringList );
private:
    // 获取光标下的文本
    QString textUnderCursor();
     //处理键盘事件
    bool eventFilter(QObject *, QEvent *);

private slots:
    // 处理补全选择的槽函数
    void changeCompletion(QString);
public slots:
    // 触发自动补全的槽函数
    void complete();
signals:
};

#endif // MYTEXTEDIT_H
