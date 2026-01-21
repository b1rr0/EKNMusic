# Создание pauseButton.png

## Проблема
Текущий файл `pauseButton.jpg` имеет черный круглый фон, который не подходит для интерфейса.

## Решение
Нужно создать `pauseButton.png` с прозрачным фоном, аналогично `playButton.png`.

## Как создать

### Вариант 1: Онлайн редактор (самый простой)
1. Откройте https://www.photopea.com/
2. File → New → 512x512px, прозрачный фон
3. Выберите инструмент Rectangle (U)
4. Нарисуйте два белых прямоугольника (паузу ||):
   - Левый: 140x380px
   - Правый: 140x380px
   - Расстояние между ними: 60px
   - Центрируйте по середине
5. File → Export As → PNG
6. Сохраните как `pauseButton.png` в `client/src/resources/images/`

### Вариант 2: Figma/Sketch
1. Создайте артборд 512x512px
2. Нарисуйте два белых прямоугольника:
   - Левый: 140x380px, закругленные углы 20px
   - Правый: 140x380px, закругленные углы 20px
   - Расстояние: 60px
3. Экспортируйте как PNG с прозрачным фоном

### Вариант 3: GIMP
1. File → New → 512x512px, Fill with: Transparency
2. Инструмент Rectangle Select
3. Создайте два прямоугольника белым цветом
4. File → Export As → pauseButton.png

### Вариант 4: Использовать готовую иконку
Скачайте бесплатную иконку паузы с:
- https://fonts.google.com/icons (Material Icons)
- https://icons8.com
- https://www.flaticon.com

Требования:
- ✅ Формат: PNG
- ✅ Фон: прозрачный
- ✅ Цвет иконки: белый (#FFFFFF)
- ✅ Размер: 512x512px (или больше)
- ✅ Стиль: два вертикальных прямоугольника ||

## Где сохранить
```
D:\programs\EKNMusic\client\src\resources\images\pauseButton.png
```

## Параметры иконки
- **Ширина**: 512px
- **Высота**: 512px
- **Фон**: прозрачный
- **Цвет**: белый (#FFFFFF)
- **Стиль**: ||
  - Левый бар: ~140x380px
  - Правый бар: ~140x380px
  - Расстояние между барами: ~60px
  - Закругленные углы: ~20px (опционально)

## Код уже обновлен
Код уже изменен на использование `pauseButton.png`:
```cpp
playPauseBtn->setIcon(QIcon(":/images/src/resources/images/pauseButton.png"));
```

После создания файла просто:
1. Скопируйте `pauseButton.png` в `client/src/resources/images/`
2. Пересоберите проект (чтобы обновился resources.qrc)
3. Запустите приложение
