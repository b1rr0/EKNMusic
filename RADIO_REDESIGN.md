# Radio Page Redesign - Implementation Summary

## Overview
Полная переработка радио-страницы с использованием наследования для поддержки нескольких радио-станций.

## Архитектура

### Базовый класс: `BaseRadioPage`
- **Файлы**: `client/src/ui/baseradiopage.h`, `client/src/ui/baseradiopage.cpp`
- **Назначение**: Абстрактный базовый класс для всех радио-станций
- **Функциональность**:
  - Фоновое изображение с обложкой альбома
  - Левая панель с текущим плеером
  - Правая панель со списком песен (история + очередь)
  - Название станции как оверлей
  - Интеграция с RadioService API

### Наследник: `EknmIntercomRadioPage`
- **Файлы**: `client/src/ui/eknmintercomradiopage.h`, `client/src/ui/eknmintercomradiopage.cpp`
- **Назначение**: Конкретная реализация для станции EKNM Intercom
- **Конфигурация**:
  - Station Name: "EKNM INTERCOM"
  - Station ID: "eknm_intercom"
  - Request Song URL: https://radio.eknm.in/public/eknm_intercom

## Новый дизайн

### Левая панель (Current Player)
- Большая обложка альбома (340x340px)
- Название песни и исполнитель
- Прогресс-бар с временем
- Кнопки управления (Play/Pause, Volume)
- Кнопки: "SONG HISTORY" и "REQUEST SONG"

### Правая панель (Song List)
- Заголовок "PLAYLIST"
- Список песен:
  - **PREVIOUS** - до 3 предыдущих песен (из history)
  - **NOW PLAYING** - текущая песня (выделена синим)
  - **NEXT** - следующая песня
  - **UPCOMING** - до 5 будущих песен (из queue)

### Дизайн изменения

#### Название радио станции
- ✅ Цвет: черный с белой обводкой
- ✅ Размер: увеличен на 10% (35px)
- ✅ Стиль: text-stroke для белой обводки

#### Удаленные элементы
- ✅ Удалена кнопка "PLAYLIST"
- ✅ Удален центральный плеер (карточка)

#### Кнопка "REQUEST SONG"
- ✅ Открывает URL в браузере: https://radio.eknm.in/public/eknm_intercom
- ✅ Использует `QDesktopServices::openUrl()`

## API Интеграция

### Endpoints используемые
1. **Now Playing**: `/api/nowplaying/eknm_intercom`
   - Обновляется каждые 10 секунд
   - Отображается в левой панели

2. **Song History**: `/api/station/eknm_intercom/history?limit=5`
   - Загружается при открытии страницы
   - Показывает 3 последние песни в списке

3. **Queue**: `/api/station/eknm_intercom/queue`
   - Обновляется каждые 10 секунд
   - Показывает до 5 будущих песен

## Для расширения в будущем

### Добавление новой радио-станции
1. Создайте новый класс, наследующий `BaseRadioPage`
2. Реализуйте методы:
   - `getStationName()` - название станции
   - `getStationId()` - ID станции для API
   - `getRequestSongUrl()` - URL для запроса песен
3. Добавьте класс в `CMakeLists.txt`
4. Используйте в `MainWindow` или другом месте

### Пример новой станции
```cpp
// newradiostation.h
class NewRadioStation : public BaseRadioPage
{
    Q_OBJECT
public:
    explicit NewRadioStation(QWidget *parent = nullptr);
    ~NewRadioStation() override;

protected:
    QString getStationName() const override { return "NEW STATION"; }
    QString getStationId() const override { return "new_station"; }
    QString getRequestSongUrl() const override { return "https://example.com/request"; }
};
```

## Файлы изменены

### Новые файлы
- `client/src/ui/baseradiopage.h`
- `client/src/ui/baseradiopage.cpp`
- `client/src/ui/eknmintercomradiopage.h`
- `client/src/ui/eknmintercomradiopage.cpp`

### Измененные файлы
- `client/CMakeLists.txt` - добавлены новые файлы
- `client/src/ui/mainwindow.h` - обновлен тип radioPage
- `client/src/ui/mainwindow.cpp` - использует EknmIntercomRadioPage

### Старые файлы (можно удалить после тестирования)
- `client/src/ui/radiopage.h` - заменен на baseradiopage.h
- `client/src/ui/radiopage.cpp` - заменен на baseradiopage.cpp

## Компиляция

```bash
cd client
cmake --build build --config Release
```

Или откройте проект в Qt Creator и соберите через IDE.

## Стилистика

### Цветовая схема
- Фон панелей: `rgba(40, 40, 50, 0.95)`
- Текущая песня: `rgba(74, 158, 255, 0.2)` (синий с прозрачностью)
- Акцентный цвет: `#4a9eff` (синий)
- Текст: белый / серый (#888, #aaa)

### Шрифты
- Название станции: 35px, bold, черный с белой обводкой
- Название песни: 14-20px, bold
- Исполнитель: 12-14px
- Метки (PREVIOUS/NOW PLAYING): 11px, bold

## Тестирование

1. Запустите приложение
2. Переключитесь в режим RADIO
3. Нажмите на EKNM INTERCOM
4. Проверьте:
   - Загружается ли фон с обложкой
   - Отображается ли текущая песня
   - Работает ли плеер (Play/Pause)
   - Обновляется ли список песен
   - Открывается ли браузер при клике на REQUEST SONG
