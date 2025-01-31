/**
* Solution to course project # 6
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semedter 2023/2024
* 
* @author Plamena Kirilova Koleva
* @idnumber 0MI0600423
* @compiler VC
* 
* <file implementing software resembling IMDB>
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>

const int MAX_FILMS = 200;
const int MAX_TITLE_LENGTH = 50;
const int MAX_GENRE_LENGTH = 20;
const int MAX_NAME_LENGTH = 50;
const int MAX_ACTORS = 3;

struct Film {
    char title[MAX_TITLE_LENGTH];
    int year;
    char genre[MAX_GENRE_LENGTH];
    char director[MAX_NAME_LENGTH];
    char actors[MAX_ACTORS][MAX_NAME_LENGTH];
    float rating;
    int ratingCount;

};

Film films[MAX_FILMS];
int filmCount = 0;


// Функция за изчисляване на дължината на char масив 
int getLength(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

// Функция за въвеждане на валиден текст
void getValidatedString(const char* prompt, char* output, int maxLength) {
    do {
        std::cout << prompt;
        std::cin.getline(output, maxLength);

        // Проверка дали низът е празен
        if (getLength(output) == 0) {
            std::cout << "Input cannot be empty! Try again.\n";
        }
    } while (getLength(output) == 0); // Продължава докато не се въведе не-празен текст
}


int getValidatedYear() {
    int year;
    do {
        std::cout << "Enter release year (1900 - 2024): ";
        std::cin >> year;
        if (std::cin.fail() || year < 1900 || year > 2024) {
            std::cout << "Invalid year! Please enter a year between 1900 and 2024.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');// чисти буфера
        }
        else {
            break;
        }
    } while (true);
    std::cin.ignore();
    return year;
}


void saveFilmToFile(const Film& newFilm) {
    std::ofstream file("films.txt", std::ios::app);
    if (!file) {
        std::cerr << "Failed to open file for writing.\n";
        return;
    }
    file << newFilm.title << "," << newFilm.year << "," << newFilm.genre << "," << newFilm.director;
    for (int i = 0; i < MAX_ACTORS; ++i) {
        file << "," << newFilm.actors[i];
    }
    file << std::endl;
    file.close();
}

void saveAllFilmsToFile() {
    std::ofstream file("films.txt", std::ios::trunc);
    if (!file) {
        std::cerr << "Failed to open file for updating.\n";
        return;
    }
    for (int i = 0; i < filmCount; ++i) {
        file << films[i].title << "," << films[i].year << "," << films[i].genre << "," << films[i].director;
        for (int j = 0; j < MAX_ACTORS; ++j) {
            file << "," << films[i].actors[j];
        }
        file << std::endl;
    }
    file.close();
}

// Функции за обработка на филми
void addFilm() {
    if (filmCount >= MAX_FILMS) {
        std::cout << "Database is full! Cannot add more films.\n";
        return;
    }
    Film newFilm;
    std::cin.ignore();
    getValidatedString("Enter film title: ", newFilm.title, MAX_TITLE_LENGTH);
    newFilm.year = getValidatedYear();
    getValidatedString("Enter genre: ", newFilm.genre, MAX_GENRE_LENGTH);
    getValidatedString("Enter director: ", newFilm.director, MAX_NAME_LENGTH);

    for (int i = 0; i < MAX_ACTORS; ++i) {
        char prompt[50];
        sprintf_s(prompt, "Enter actor %d: ", i + 1);
        getValidatedString(prompt, newFilm.actors[i], MAX_NAME_LENGTH);
    }

    films[filmCount++] = newFilm;
    saveFilmToFile(newFilm);
    std::cout << "Film added successfully!\n";
}

void listFilms() {
    if (filmCount == 0) {
        std::cout << "No movies available.\n";
        return;
    }
    for (int i = 0; i < filmCount; ++i) {
        std::cout << i + 1 << ". " << films[i].title << " (" << films[i].year << ") - "
            << films[i].genre << " - Director: " << films[i].director
            << " - Rating: " << (films[i].ratingCount > 0 ? films[i].rating / films[i].ratingCount : 0)
            << std::endl;
    }
}

void deleteFilm() {
    if (filmCount == 0) {
        std::cout << "No films to delete.\n";
        return;
    }
    listFilms();
    int index;
    std::cout << "Enter the number of the film to delete: ";
    std::cin >> index;
    if (index < 1 || index > filmCount) {
        std::cout << "Invalid choice!\n";
        return;
    }
    for (int i = index - 1; i < filmCount - 1; ++i) {
        films[i] = films[i + 1];
    }
    --filmCount;
    saveAllFilmsToFile();
    std::cout << "Film deleted successfully!\n";
}

// Функции за сортиране
int compareStrings(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] < str2[i]) return -1; // `str1` идва преди `str2`
        if (str1[i] > str2[i]) return 1;  // `str1` идва след `str2`
        i++;
    }
    if (str1[i] == '\0' && str2[i] == '\0') return 0; // Равни
    if (str1[i] == '\0') return -1; // `str1` е по-кратък -> идва първи
    return 1; // `str2` е по-кратък -> идва първи
}

void sortByTitle() {
    for (int i = 0; i < filmCount - 1; ++i) {
        for (int j = 0; j < filmCount - i - 1; ++j) {
            if (compareStrings(films[j].title, films[j + 1].title) > 0) {
                Film temp = films[j];
                films[j] = films[j + 1];
                films[j + 1] = temp;
            }
        }
    }
}


void sortByRating() {
    for (int i = 0; i < filmCount - 1; ++i) {
        for (int j = 0; j < filmCount - i - 1; ++j) {
            float ratingA = (films[j].ratingCount > 0 ? films[j].rating / films[j].ratingCount : 0);
            float ratingB = (films[j + 1].ratingCount > 0 ? films[j + 1].rating / films[j + 1].ratingCount : 0);
            if (ratingA < ratingB) {
                Film temp = films[j];
                films[j] = films[j + 1];
                films[j + 1] = temp;
            }
        }
    }
}

// Функция за търсене на филм по жанр
void searchByGenre() {
    if (filmCount == 0) {
        std::cout << "No movies found." << std::endl;
        return;
    }

    char genre[MAX_GENRE_LENGTH];
    std::cout << "Enter genre: ";
    std::cin.ignore();
    std::cin.getline(genre, MAX_GENRE_LENGTH);

    bool found = false;
    for (int i = 0; i < filmCount; ++i) {
        if (compareStrings(films[i].genre, genre) == 0) {
            std::cout << i + 1 << ". " << films[i].title << " (" << films[i].year << ") - "
                << films[i].director << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No movies with this genre: \"" << genre << "\" found." << std::endl;
    }
}
// Функция за търсене на филм по заглавие
bool containsSubstring(const char* text, const char* pattern) {
    int textLen = 0, patternLen = 0;

    while (text[textLen] != '\0') {
        textLen++;
    }

    while (pattern[patternLen] != '\0') {
        patternLen++;
    }

    for (int i = 0; i <= textLen - patternLen; i++) {
        bool match = true;
        for (int j = 0; j < patternLen; j++) {
            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return true;
        }
    }
    return false;
}


void searchByTitle() {
    if (filmCount == 0) {
        std::cout << "No movies found." << std::endl;
        return;
    }

    char titlePart[MAX_TITLE_LENGTH];
    std::cout << "Enter part of the title: ";
    std::cin.ignore();
    std::cin.getline(titlePart, MAX_TITLE_LENGTH);

    bool found = false;
    for (int i = 0; i < filmCount; ++i) {
        if (containsSubstring(films[i].title, titlePart)) {
            std::cout << i + 1 << ". " << films[i].title << " (" << films[i].year << ") - "
                << films[i].genre << " - Director: " << films[i].director << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No movies containing \"" << titlePart << "\" found." << std::endl;
    }
}


void editFilm() {
    if (filmCount == 0) {
        std::cout << "No films available to edit.\n";
        return;
    }

    listFilms();
    int choice;
    std::cout << "Enter the number of the film to edit: ";
    std::cin >> choice;

    if (choice < 1 || choice > filmCount) {
        std::cout << "Invalid choice!\n";
        return;
    }

    Film& selectedFilm = films[choice - 1];

    int editChoice;
    do {
        std::cout << "\nEditing Film: " << selectedFilm.title << std::endl;
        std::cout << "1. Edit Title\n2. Edit Year\n3. Edit Genre\n4. Edit Director\n";
        std::cout << "5. Edit Actors\n6. Edit Rating\n7. Exit Editing\nEnter your choice: ";
        std::cin >> editChoice;
        std::cin.ignore();

        switch (editChoice) {
        case 1:
            getValidatedString("Enter new title: ", selectedFilm.title, MAX_TITLE_LENGTH);
            break;
        case 2:
            selectedFilm.year = getValidatedYear();
            break;
        case 3:
            getValidatedString("Enter new genre: ", selectedFilm.genre, MAX_GENRE_LENGTH);
            break;
        case 4:
            getValidatedString("Enter new director: ", selectedFilm.director, MAX_NAME_LENGTH);
            break;
        case 5:
            for (int i = 0; i < MAX_ACTORS; ++i) {
                char prompt[30];
                sprintf_s(prompt, "Enter new actor %d: ", i + 1);
                getValidatedString(prompt, selectedFilm.actors[i], MAX_NAME_LENGTH);
            }
            break;
        case 6:
            std::cout << "Enter new rating (total score): ";
            std::cin >> selectedFilm.rating;
            std::cout << "Enter new rating count: ";
            std::cin >> selectedFilm.ratingCount;
            std::cin.ignore();
            break;
        case 7:
            std::cout << "Exiting editing menu...\n";
            return;
        default:
            std::cout << "Invalid choice! Please try again.\n";
        }
        std::cout << "Update successful!\n";
    } while (true);
}

// Функция за оценяване на филм
void rateFilm() {
    if (filmCount == 0) {
        std::cout << "There aren't any movies to rate." << std::endl;
        return;
    }
    listFilms();
    int choice;
    std::cout << "Choose movie to rate: (1-" << filmCount << "): ";
    std::cin >> choice;
    if (choice < 1 || choice > filmCount) {
        std::cout << "Invalid choise!" << std::endl;
        return;
    }
    float rating;
    std::cout << "Rate movie: (1-10): ";
    std::cin >> rating;
    if (rating < 1 || rating > 10) {
        std::cout << "Invalid rating!" << std::endl;
        return;
    }
    films[choice - 1].rating += rating;
    films[choice - 1].ratingCount += 1;
    float averageRating = films[choice - 1].rating / films[choice - 1].ratingCount;

    std::cout << "Thanks for rating!" << std::endl;
    std::cout << "The updated average rating for \"" << films[choice - 1].title
        << "\" is: " << averageRating << std::endl;
}

// Функция за филтриране на филми по минимален рейтинг
void filterByRating() {
    if (filmCount == 0) {
        std::cout << "No films available.\n";
        return;
    }

    float minRating;
    std::cout << "Enter minimum rating: ";
    std::cin >> minRating;

    bool found = false;
    for (int i = 0; i < filmCount; ++i) {
        float avgRating = (films[i].ratingCount > 0 ? films[i].rating / films[i].ratingCount : 0);
        if (avgRating >= minRating) {
            std::cout << i + 1 << ". " << films[i].title << " (" << films[i].year << ") - "
                << films[i].genre << " - Director: " << films[i].director
                << " - Rating: " << avgRating << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No films found with rating greater than or equal to " << minRating << ".\n";
    }
}

void loadFilmsFromFile(const char* fileName) {
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }

    char line[256]; // Лимит за един ред
    filmCount = 0;  // Нулираме броя на филмите

    while (file.getline(line, sizeof(line)) && filmCount < MAX_FILMS) {
        Film newFilm;
        char* next_token = nullptr;
        char* token = strtok_s(line, ",", &next_token);

        if (!token) continue;  // Ако редът е празен, пропускаме

        strncpy_s(newFilm.title, token, MAX_TITLE_LENGTH - 1);

        token = strtok_s(nullptr, ",", &next_token);
        if (!token) continue;
        newFilm.year = atoi(token);

        token = strtok_s(nullptr, ",", &next_token);
        if (!token) continue;
        strncpy_s(newFilm.genre, token, MAX_GENRE_LENGTH - 1);

        token = strtok_s(nullptr, ",", &next_token);
        if (!token) continue;
        strncpy_s(newFilm.director, token, MAX_NAME_LENGTH - 1);

        for (int i = 0; i < MAX_ACTORS; ++i) {
            token = strtok_s(nullptr, ",", &next_token);
            if (token) {
                strncpy_s(newFilm.actors[i], token, MAX_NAME_LENGTH - 1);
            }
            else {
                strcpy_s(newFilm.actors[i], MAX_NAME_LENGTH, "Unknown");
            }
        }
        token = strtok_s(nullptr, ",", &next_token);
        if (token) newFilm.rating = atof(token);

        token = strtok_s(nullptr, ",", &next_token);
        if (token) newFilm.ratingCount = atoi(token);

        films[filmCount++] = newFilm;
    }

    file.close();
    std::cout << "Successfully loaded " << filmCount << " films from file.\n";
}



int main() {
    loadFilmsFromFile("films.txt");  // Зареждане на филмите от файла

    int userType;
    std::cout << "Choose type of user:\n1. Administrator\n2. Normal user\nYour choice: ";
    std::cin >> userType;

    if (userType == 1) {  // Меню за администратор
        int choice;
        do {
            std::cout << "\nAdmin Menu:" << std::endl;
            std::cout << "1. Add new film" << std::endl;
            std::cout << "2. Delete a film" << std::endl;
            std::cout << "3. Search movie by genre" << std::endl;
            std::cout << "4. Search movie by title" << std::endl;
            std::cout << "5. List all films" << std::endl;
            std::cout << "6. Change movie info" << std::endl;
            std::cout << "7. Sort films (by title or rating)" << std::endl;
            std::cout << "8. Exit" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1: addFilm(); break;
            case 2: deleteFilm(); break;
            case 3: searchByGenre(); break;
            case 4: searchByTitle(); break;
            case 5: listFilms(); break;
            case 6: editFilm(); break;
            case 7:
                int sortChoice;
                std::cout << "Sort movies by:\n1. Title\n2. Rating\nYour choice: ";
                std::cin >> sortChoice;
                if (sortChoice == 1) sortByTitle();
                else if (sortChoice == 2) sortByRating();
                else std::cout << "Invalid choice!\n";
                listFilms();
                break;
            case 8: std::cout << "Exiting admin menu..." << std::endl; break;
            default: std::cout << "Invalid choice. Try again!\n";
            }
        } while (choice != 8);
    }

    else if (userType == 2) {  // Меню за нормален потребител
        int choice;
        do {
            std::cout << "\nUser Menu:" << std::endl;
            std::cout << "1. List of movies" << std::endl;
            std::cout << "2. Rate movie" << std::endl;
            std::cout << "3. Search movie by genre" << std::endl;
            std::cout << "4. Search movie by title" << std::endl;
            std::cout << "5. Sort movies" << std::endl;
            std::cout << "6. Filter movies by rating" << std::endl;
            std::cout << "7. Exit" << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> choice;

            switch (choice) {
            case 1: listFilms(); break;
            case 2: rateFilm(); break;
            case 3: searchByGenre(); break;
            case 4: searchByTitle(); break;
            case 5:
                int sortChoice;
                std::cout << "Sort movies by:\n1. Title\n2. Rating\nYour choice: ";
                std::cin >> sortChoice;
                if (sortChoice == 1) sortByTitle();
                else if (sortChoice == 2) sortByRating();
                else std::cout << "Invalid choice!\n";
                listFilms();
                break;
            case 6: filterByRating(); break;
            case 7: std::cout << "Exiting system...\n"; break;
            default: std::cout << "Invalid choice. Try again!\n";
            }
        } while (choice != 7);
    }

    else {
        std::cout << "Invalid choice!\n";
    }

    return 0;
}
