# Bug Fix: Radio Page Crash on Navigation

## Problem
Приложение крашилось при переходе от логина к основным экранам из-за вызова виртуальных методов в конструкторе базового класса.

## Root Cause
В C++ виртуальные методы не работают полиморфно в конструкторах и деструкторах. Когда `BaseRadioPage` вызывал `getStationName()` в методе `setupBackgroundLayer()` (который вызывается из конструктора), производный класс `EknmIntercomRadioPage` еще не был полностью сконструирован.

## Solution

### 1. Отложенная инициализация названия станции
**Файл**: `client/src/ui/baseradiopage.cpp`

**Было (строка 67)**:
```cpp
stationTitle = new QLabel(getStationName(), this);
```

**Стало**:
```cpp
stationTitle = new QLabel("", this); // Пустая строка при создании
```

### 2. Установка названия после полной конструкции
**Файл**: `client/src/ui/baseradiopage.cpp` - метод `loadRadioBackground()`

**Добавлено**:
```cpp
void BaseRadioPage::loadRadioBackground()
{
    // Set station title now that the derived class is fully constructed
    stationTitle->setText(getStationName());

    // ... rest of the method
}
```

### 3. Добавлен обработчик resizeEvent
**Файл**: `client/src/ui/baseradiopage.h` и `baseradiopage.cpp`

Добавлен метод `resizeEvent()` для корректного изменения размеров фоновых элементов:
```cpp
void BaseRadioPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Update background and overlay sizes
    if (backgroundLabel) {
        backgroundLabel->setGeometry(0, 0, width(), height());
    }
    if (overlayWidget) {
        overlayWidget->setGeometry(0, 0, width(), height());
    }
    if (stationTitle) {
        stationTitle->setGeometry(0, 30, width(), 50);
    }
}
```

## Why This Works
1. **Отложенная инициализация**: Название станции устанавливается в `loadRadioBackground()`, который вызывается из `MainWindow::showRadio()` ПОСЛЕ полной конструкции объекта
2. **Безопасный вызов**: В момент вызова `loadRadioBackground()` объект `EknmIntercomRadioPage` полностью сконструирован, и виртуальный метод `getStationName()` работает корректно
3. **Null checks**: В `resizeEvent()` проверяем указатели перед использованием

## Files Modified
- `client/src/ui/baseradiopage.h` - добавлен `resizeEvent()`
- `client/src/ui/baseradiopage.cpp` - исправлена инициализация и добавлен resizeEvent

## Testing
1. Запустите приложение
2. Войдите в систему
3. Переключитесь в режим RADIO
4. Приложение должно работать без крашей
5. Название станции должно отображаться корректно

## C++ Best Practice
**Правило**: Никогда не вызывайте виртуальные методы в конструкторах или деструкторах.

**Почему**: В конструкторе базового класса vtable производного класса еще не инициализирована, поэтому вызов виртуального метода не будет полиморфным и может привести к неопределенному поведению или крашу.

**Решение**: Используйте двухфазную инициализацию (конструктор + init/load метод).
