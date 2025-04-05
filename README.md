# Hamming Archiver

## Задача

Реализовать помехоустойчивый архиватор файлов без сжатия **HamArc**, объединяющий несколько файлов в один архив. Для помехоустойчивого кодирования использовать [Коды Хэмминга](https://en.wikipedia.org/wiki/Hamming_code). Формат хранения данных в архиве также является частью задачи.

## Требования

* Объединять несколько файлов ( >= 1) в один архив в формате .haf (Hamming Archive File)
* Извлекать все или отдельные файлы из архива
* Объединять несколько архивов в один
* Восстанавливать архив при повреждениях, либо сообщать о том что это невозможно
* Возвращать список файлов в архиве

## Реализация

Консольное приложение, поддерживающее следующие аргументы командной строки:

**-c, --create**           - создание нового архива 

**-f, --file=[ARHCNAME]**  - имя файла с архивом 

**-l, --list**             - вывести список файлов в архиве

**-x, --extract**          - извлечь файлы из архива  (если не указано, то все файлы)

**-a, --append**           - добавить файл в архив

**-d, --delete**           - удалить файл из архива

**-A, --merge**      - смерджить два архива

**Имена файлов передаются свободными аргументами**

**Аргументы для кодирования и декодирования так же передаются через командую строку** (Названия и типы аргументов часть задания)

### Примеры запуска
   
*hamarc --create --file=ARCHIVE FILE1 FILE2 FILE3*
   
*hamarc -l -f ARCHIVE*
   
*hamarc --concantenate  ARCHIVE1 ARCHIVE2 -f ARCHIVE3*
