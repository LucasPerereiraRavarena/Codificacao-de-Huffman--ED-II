#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <locale>
#include <codecvt>
#include <string>
#include <unordered_map>
#include <queue>
#include <memory>

std::wstring get_readable_character(wchar_t ch) {
  switch (ch) {
    case L' ': return L"Espaço";
    case L'\n': return L"Nova Linha";
    case L'\t': return L"Tabulação";
    default: return std::wstring(1, ch);
  }
}
// Estrutura para o nó da árvore de Huffman
struct HuffmanNode {
    wchar_t character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    
    HuffmanNode(wchar_t ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
    HuffmanNode(HuffmanNode* l, HuffmanNode* r) : character(0), frequency(l->frequency + r->frequency), left(l), right(r) {}
};

// Função de comparação para a fila de prioridade
struct Compare {
    bool operator()(HuffmanNode* const& a, HuffmanNode* const& b) {
        return a->frequency > b->frequency;
    }
};

// Função para gerar o código de Huffman
void generateHuffmanCodes(HuffmanNode* node, std::wstring code, std::unordered_map<wchar_t, std::wstring>& huffmanCodes) {
    if (!node) return;
    if (!node->left && !node->right) {
        huffmanCodes[node->character] = code;
    }
    generateHuffmanCodes(node->left, code + L"0", huffmanCodes);
    generateHuffmanCodes(node->right, code + L"1", huffmanCodes);
}


// Função para desalocar a memória da árvore de Huffman
void deleteHuffmanTree(HuffmanNode* node) {
    if (node) {
        deleteHuffmanTree(node->left);
        deleteHuffmanTree(node->right);
        delete node;
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

     // Construção da árvore de Huffman
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;
    for (const auto& pair : chars) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* parent = new HuffmanNode(left, right);
        pq.push(parent);
    }

    HuffmanNode* root = pq.top();

    // Geração dos códigos de Huffman
    std::unordered_map<wchar_t, std::wstring> huffmanCodes;
    generateHuffmanCodes(root, L"", huffmanCodes);


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
    html << "<tr><th>Caractere </th><th>Frequencia</th><th>Codigo de Huffman</th></tr>\n";
  
     
    
    for (auto it : vi) {
        html << "<tr><td>"
             << get_readable_character(it->first)
             << "</td><td>" 
             << it->second 
             << "</td><td>" 
             << huffmanCodes[it->first]
             << "</td><tr>\n";
    }
    html << "</table>\n";

    html << "<h2> Arvore de Huffman </h2>\n";
    
    

    html << "</table>\n";
    html << "</body>\n";
    html << "</html>\n";
    html.close();
    
    system("xdg-open characters.html");

    deleteHuffmanTree(root);
    
}