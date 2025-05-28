#include "Player_Create.h"
#include <QInputDialog>
#include <QMessageBox>

Player* PlayerCreate::createNewPlayer(QWidget* parent) {
    bool ok;

    QString name = QInputDialog::getText(parent,
                                         "Новый игрок",
                                         "Введите имя:",
                                         QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty()) {
        QMessageBox::warning(parent, "Ошибка", "Имя не может быть пустым!");
        return nullptr;
    }

    int age = QInputDialog::getInt(parent,
                                   "Новый игрок",
                                   "Введите возраст:",
                                   20, 16, 50, 1, &ok);
    if (!ok) return nullptr;

    QString position = QInputDialog::getText(parent,
                                             "Новый игрок",
                                             "Введите позицию:",
                                             QLineEdit::Normal, "", &ok);
    if (!ok || position.trimmed().isEmpty()) {
        QMessageBox::warning(parent, "Ошибка", "Позиция не может быть пустой!");
        return nullptr;
    }

    int dribbling = QInputDialog::getInt(parent, "Новый игрок", "Дриблинг:", 70, 0, 100, 1, &ok);
    if (!ok) return nullptr;

    int defense = QInputDialog::getInt(parent, "Новый игрок", "Защита:", 70, 0, 100, 1, &ok);
    if (!ok) return nullptr;

    int shot = QInputDialog::getInt(parent, "Новый игрок", "Удар:", 70, 0, 100, 1, &ok);
    if (!ok) return nullptr;

    int speed = QInputDialog::getInt(parent, "Новый игрок", "Скорость:", 70, 0, 100, 1, &ok);
    if (!ok) return nullptr;

    int pass = QInputDialog::getInt(parent, "Новый игрок", "Пас:", 70, 0, 100, 1, &ok);
    if (!ok) return nullptr;

    int endurance = QInputDialog::getInt(parent, "Новый игрок", "Выносливость:", 70, 0, 100, 1, &ok);
    if (!ok) return nullptr;

    int physicalStrength = QInputDialog::getInt(parent, "Новый игрок", "Физическая сила:", 70, 0, 100, 1, &ok);
    if (!ok) return nullptr;

    int currentForm = QInputDialog::getInt(parent, "Новый игрок", "Форма:", 70, 0, 100, 1, &ok);
    if (!ok) return nullptr;

    int price = QInputDialog::getInt(parent, "Новый игрок", "Цена:", 200, 0, 1000, 1, &ok);
    if (!ok) return nullptr;

    return new Player(name, age, position, dribbling, defense, shot, speed, pass,
                      endurance, physicalStrength, currentForm, price);
}
