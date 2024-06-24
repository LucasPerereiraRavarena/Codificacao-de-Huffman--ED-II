// Segunda Versão do projeto.
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

 // Cria wifstream e configura a locale para UTF-8
    std::wifstream arq(a);
    arq.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

  if (!arq) {
    std::cout << "open() falhou\n";
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
 
 // Configura o locale para UTF-8 para o wofstream
  std::wofstream csv("caracteres.csv");
  csv.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

  csv << "Caractere, N\n";

  for(auto it : vi){
    csv << get_readable_character(it->first) << ", " << it->second << std::endl;
  }
  csv.close();
}