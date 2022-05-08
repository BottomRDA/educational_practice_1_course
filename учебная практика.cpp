#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <locale>
#include <cctype>
#include <ctime>
using namespace std;

//поиск элемента в алфавите
int alphabet(char first_symbol)
{
    char letters [43] = {'а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ','ъ','ы','ь', 'э', 'ю', 'я', '0','1','2','3','4','5','6','7','8','9'};
    for (int i = 0; i < 42; i++)
    {
        if (first_symbol == letters[i])
            return i;
    }
}
//анализ текста
vector <long long> analis( vector <string> sorted_word)
{
    //Преобразование в маленькие буквы
    for (int i = 0; i < sorted_word.size(); i++)
    {
        sorted_word[i][0] = tolower(sorted_word[i][0]);
    }
    //запись в массив количества встреченных букв
    vector <long long> numbers(43);
    for (int i = 0; i < sorted_word.size(); i++)
    {
        int j = alphabet(sorted_word[i][0]);
        numbers[j]++;
        j = 0;
    }
    return numbers;
}
//Сортировка Шелла
vector <string> sort_shell(list <string> updated_line_word)
{
    vector<string> sorted_word(updated_line_word.begin(), updated_line_word.end());
    int j;
    for (int step = sorted_word.size() / 2; step > 0; step /= 2)
        for (int i = step; i < sorted_word.size(); i++)
        {
            string tmp = sorted_word[i];
            for (j = i; j >= step; j -= step)
            {
                if (tmp[0] < sorted_word[j - step][0])
                    sorted_word[j] = sorted_word[j - step];
                else
                    break;
            }
            sorted_word[j] = tmp;
        }
    return sorted_word;
}
//Удаление лишних строк
list <string> clear_of_line(list <string> updated_word)
{
    list < string> updated_line_word;
    auto p = updated_word.begin();
    for (p; p != updated_word.end(); p++)
    {
        if (*p == "")
        {
            updated_word.erase(p--);
        }
    }
    updated_line_word = updated_word;
    return updated_line_word;
}
//Очищение лишних символов
list<string> clear_of_excess(list<string> words)
{
    list<string> updated_words;
    vector<char> symbols_of_updated_word;

    for (string word : words)
    {
        for (char symbol : word)
        {
            if ((symbol >= 'А' && symbol <= 'я') || (symbol >= '0' && symbol <= '9') || ((symbol == ',' || symbol == '.') && (symbols_of_updated_word.size() > 0 && symbols_of_updated_word.back() >= '0' && symbols_of_updated_word.back() <= '9')))
                symbols_of_updated_word.push_back(symbol);

        }
        string updated_word(symbols_of_updated_word.begin(), symbols_of_updated_word.end());
        updated_words.push_back(updated_word);
        symbols_of_updated_word.clear();
    }
    return updated_words;
}
//Разделение слов
list <string> division_into_words(string text)
{
    list<string> words;
    vector <char> symbols_of_word;

    for (char symbol : text)
    {
        if (symbol == ' ' || symbol == '\n')
        {
            string word(symbols_of_word.begin(), symbols_of_word.end());
            words.push_back(word);
            symbols_of_word.clear();
        }
        else {
            symbols_of_word.push_back(symbol);
        }
    }
    return words;
}
//Вызов сортировки и измерение времени сортировки
unsigned int time_sort(list <string> updated_line_word)
{
    unsigned int start_time = clock();
    sort_shell(updated_line_word);
    unsigned int end_time = clock();
    return end_time - start_time;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    string text, fileName;
    cout << "Введите путь файла: ";
    cin >> fileName;
    //чтение из файла
    ifstream inText;
    inText.open(fileName);
    if (inText.is_open())
    {
        string temp;
        while (getline(inText, temp))
        {
            text += temp;
            if (!inText.eof())
                text += '\n';
        }
        inText.close();
    }
    else
        cout << " Unable to open file." << endl;

    list <string> div = division_into_words(text);

    list <string> clear = clear_of_excess(div);

    list <string> clear_line = clear_of_line(clear);

    vector <string> sort = sort_shell(clear_line);

    unsigned int time = time_sort(clear_line);

    vector<long long> analisis = analis(sort);

    //Вывод в файл
    ofstream sortText;          // поток для записи
    sortText.open("Source\\sortText.txt"); // окрываем файл для записи
    for (auto iter = sort.begin(); iter != sort.end(); iter++)
    {
        sortText << *iter << "\n";
    }
    sortText.close();

    char letters[43] = { 'а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ','ъ','ы','ь', 'э', 'ю', 'я', '0','1','2','3','4','5','6','7','8','9' };
    //Вывод в файл
    ofstream analisText;          // поток для записи
    analisText.open("Source\\analisText.txt"); // окрываем файл для записи
    analisText << text<<endl;
    analisText <<"Задание."<< endl << "Вариант 22. Кириллица, по алфавиту, по возрастанию, учитывать числа, сортировка Шелла" << endl;
    analisText << "Количество слов в тексте: " << sort.size()<< endl;
    analisText << "Время сортировки: " << time << " миллисекунд"<< endl<< "Статистика (количество слов на каждую букву алфавита): \n";

    for (int i = 0; i < analisis.size(); i++)
    {
        analisText <<letters[i]<< " - " << analisis[i] << endl;
    }
    analisText.close();
    cout << "Программа успешно завершилась!";
    
    return 0;
}
