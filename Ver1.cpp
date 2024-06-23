// Primeira Versão do projeto.
#include <cstdlib>  
#include <fstream>
#include <iostream>
#include <vector>
#include <locale>
#include <codecvt>
#include <string>

struct chr {
  wchar_t character;
  int count;
};

int main() {

    std::cout << "Digite o nome do arquivo de texto que deseja ler (Ex: arquivo.txt): "; 
    std::string a;
    std::cin >> a;
    std::cout << "\n";

 // Cria um wifstream e configura a locale para UTF-8
    std::wifstream arq(a);
    arq.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

  if (!arq) {
    std::cout << "open() falhou\n";
    exit(1);
  }

  wchar_t ch;
  std::vector<chr> chars;

  while (arq.get(ch)) {
      bool found = false;
      for (int i = 0; i < chars.size(); i++) {
        if (chars[i].character == ch) {
          chars[i].count++;
          found = true;
          break;
        }
      }
      if (!found) {
        chars.push_back({ch, 1});
      }
  }
  arq.close();

  std::cout << chars.size() << " caracteres distintos encontrados\n";
 
 // Configura o locale para wofstream
  std::locale::global(std::locale(""));
  std::wofstream csv("caracteres.csv");
  csv.imbue(std::locale(""));

  csv << "Caractere, N\n";

  for(auto c: chars){
    csv << c.character << ", " << c.count << std::endl;
  }
  csv.close();
}