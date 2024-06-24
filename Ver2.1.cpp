// Versão 2.1 do projeto.
#include <algorithm>
#include <cstdlib>  
#include <fstream>
#include <iostream>
#include <vector>
#include <locale>
#include <codecvt>
#include <string>
#include <unordered_map>

std::wstring get_readable_character(wchar_t ch) {
  switch (ch) {
    case L' ': return L"Espaço";
    case L'\n': return L"Nova Linha";
    case L'\t': return L"Tabulação";
    default: return std::wstring(1, ch);
  }
}

int main() {
    std::cout << "Digite o nome do arquivo de texto que deseja ler (Ex: arquivo.txt): "; 
    std::string a;
    std::cin >> a;
    std::cout << "\n";

    // Cria um wifstream e configura a locale para UTF-8
    std::wifstream arq(a);
    arq.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    if (!arq) {
        std::cerr << "open() falhou\n";
        exit(1);
    }

    wchar_t ch;
    std::unordered_map<wchar_t, int> chars;

    while (arq.get(ch)) {
        chars[ch]++;
    }

    arq.close();

    std::cout << chars.size() << " caracteres distintos encontrados\n";

    // Ordenação indireta
    std::vector<std::unordered_map<wchar_t, int>::iterator> vi;
    vi.reserve(chars.size());
    for (auto it = chars.begin(); it != chars.end(); ++it) {
        vi.push_back(it);
    }

    sort(vi.begin(), vi.end(), [](auto a, auto b) {
        return (a->second > b->second) ||
               (a->second == b->second && a->first < b->first);
    });


std::wofstream html("characters.html");
// Configura o locale para UTF-8 para o wofstream
html.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "<title> Characters " << "</title>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "<H1> Characters " << "</H1>\n";
    html << "<table border=1>\n";
    
    for (auto it : vi) {
        html << "<tr><td>"
             << get_readable_character(it->first)
             << "</td><td>" 
             << it->second 
             << "</td><tr>\n";
    }
    
    html << "</table>\n";
    html << "</body>\n";
    html << "</html>\n";
    html.close();
    
    system("xdg-open characters.html");
}
