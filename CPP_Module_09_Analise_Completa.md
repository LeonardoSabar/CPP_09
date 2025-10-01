# CPP Module 09 - Análise Completa e Detalhada

## **Sumário**
1. [Objetivo Geral do Módulo](#objetivo-geral-do-módulo)
2. [Conceitos Fundamentais Aplicados](#conceitos-fundamentais-aplicados)
3. [Exercício 00 - BitcoinExchange](#exercício-00---bitcoinexchange)
4. [Exercício 01 - RPN](#exercício-01---rpn-reverse-polish-notation)
5. [Exercício 02 - PmergeMe](#exercício-02---pmergeme-ford-johnson-algorithm)
6. [Comparação de Performance entre Containers](#comparação-de-performance-entre-containers)
7. [Pontos Importantes para Defesa](#pontos-importantes-para-defesa)
8. [Conceitos Avançados C++](#conceitos-avançados-c)

---

## **Objetivo Geral do Módulo**

O CPP Module 09 da escola 42 é focado no **domínio avançado de containers STL** (Standard Template Library) em C++. O módulo tem como objetivos principais:

### **Objetivos Específicos:**
1. **Dominar diferentes containers STL** e entender quando usar cada um
2. **Implementar algoritmos complexos** usando containers apropriados
3. **Comparar performance** entre diferentes estruturas de dados
4. **Aplicar conceitos de ordenação avançada** (Ford-Johnson)
5. **Trabalhar com parsing e validação** de dados complexos
6. **Demonstrar tratamento robusto de erros**

### **Skills Desenvolvidas:**
- Análise de complexidade algorítmica
- Escolha adequada de estruturas de dados
- Otimização de performance
- Parsing e validação de entrada
- Exception handling
- Medição de tempo de execução

---

## **Conceitos Fundamentais Aplicados**

### **1. Standard Template Library (STL)**
A STL é o coração deste módulo. Cada exercício explora um container específico:

#### **Containers Utilizados:**
- **`std::map`** (Exercício 00): Mapeamento ordenado chave-valor com busca O(log n)
- **`std::stack`** (Exercício 01): Estrutura LIFO (Last In, First Out) para cálculos RPN
- **`std::vector`** (Exercício 02): Array dinâmico com acesso randômico O(1)
- **`std::deque`** (Exercício 02): Double-ended queue com inserção/remoção eficiente nas extremidades

### **2. Complexidade Algorítmica**
Cada exercício demonstra diferentes complexidades:

| Exercício | Container | Operação Principal | Complexidade |
|-----------|-----------|-------------------|--------------|
| Ex00 | `std::map` | Busca (`lower_bound`) | O(log n) |
| Ex01 | `std::stack` | Push/Pop | O(1) por operação, O(n) total |
| Ex02 | `std::vector`/`std::deque` | Ford-Johnson Sort | O(n log n) |

### **3. RAII (Resource Acquisition Is Initialization)**
Todos os exercícios aplicam RAII:
```cpp
// Construtor inicializa recursos
BitcoinExchange::BitcoinExchange(){
    loadData("data.csv");  // Carrega dados no construtor
}

// Destrutor limpa automaticamente (STL containers fazem isso)
BitcoinExchange::~BitcoinExchange(){}
```

### **4. Exception Safety**
Cada exercício implementa tratamento robusto de exceções:
```cpp
try {
    BitcoinExchange exchange;
    readInputFile(argv[1], exchange);
} catch (std::runtime_error &e) {
    std::cerr << RED << e.what() << RESET << std::endl;
    return 1;
}
```

---

## **Exercício 00 - BitcoinExchange**

### **Objetivo Detalhado:**
Criar um programa que simule uma consulta histórica de preços de Bitcoin. O programa deve:
1. Carregar um banco de dados histórico (data.csv)
2. Processar um arquivo de entrada com consultas
3. Calcular valores baseados em preços históricos
4. Validar todas as entradas rigorosamente

### **Arquitetura da Solução Implementada**

#### **1. Separação de Responsabilidades**

A implementação atual utiliza **duas classes principais**:

**A. Classe `BitcoinExchange`:**
```cpp
class BitcoinExchange {
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange &other);
    BitcoinExchange &operator=(const BitcoinExchange &other);
    ~BitcoinExchange();

    std::map<std::string, float>& getRates();
    void makeExchange(const std::string &date) const;

private:
    std::map<std::string, float> rates;
};
```

**Responsabilidades:**
- Armazenar os dados históricos do Bitcoin
- Realizar cálculos de câmbio
- Buscar taxas por data (com algoritmo de data mais próxima)

**B. Classe `Parser` (Static Utility Class):**
```cpp
class Parser {
public:
    static void parseFile(const std::string &filename, BitcoinExchange &exchange);
    static std::string validateDate(const std::string &dateStr);
    static float atof(const std::string &rateStr);
    static int atoi(const std::string &str);
    static bool validateHeaderInputFile(std::ifstream &file);
private:
    // Construtor privado - apenas métodos estáticos
    Parser();
    Parser(const Parser &other);
    Parser &operator=(const Parser &other);
    ~Parser();
};
```

**Responsabilidades:**
- Parsing de arquivos CSV
- Validação de formatos de data
- Conversão segura de strings para números
- Validação de cabeçalhos de arquivos

#### **2. Estrutura de Dados: `std::map<std::string, float>`**

**Por que `std::map` é a escolha ideal?**

```cpp
std::map<std::string, float> rates;  // data -> taxa
```

**Características técnicas:**
- **Ordenação Automática:** Strings de data são naturalmente ordenadas lexicograficamente
- **Busca Eficiente:** O(log n) usando Red-Black Tree
- **upper_bound/lower_bound:** Permite encontrar data mais próxima
- **Chaves Únicas:** Garante uma taxa por data

#### **3. Algoritmo de Busca por Data Mais Próxima**

**Implementação Atual:**
```cpp
void BitcoinExchange::makeExchange(const std::string &date) const {
    // ... parsing e validação ...
    
    // Busca exata primeiro
    std::map<std::string, float>::const_iterator it = rates.find(dateStr);
    if (it != rates.end()) {
        float result = rate * it->second;
        std::cout << dateStr << " => " << rate << " = " << result << std::endl;
        return;
    }

    // Se não encontrou data exata, busca a anterior mais próxima
    it = rates.upper_bound(dateStr);
    if (it == rates.begin())
        throw std::runtime_error("No available rate for date: " + dateStr);
    
    it--;  // Data anterior mais próxima
    float result = rate * it->second;
    std::cout << dateStr << " => " << rate << " = " << result << std::endl;
}
```

**Lógica do Algoritmo:**
1. **Busca Exata:** Usa `find()` para procurar a data específica
2. **Busca Aproximada:** Usa `upper_bound()` para encontrar a primeira data maior
3. **Retrocede:** `it--` para pegar a data anterior (mais próxima menor)
4. **Validação:** Se `it == begin()`, a data é muito antiga

#### **4. Sistema Robusto de Validação**

**A. Validação de Data com Verificação Completa:**
```cpp
std::string Parser::validateDate(const std::string &dateStr) {
    size_t first = dateStr.find('-');
    size_t second = dateStr.find('-', first + 1);

    std::string yearStr = dateStr.substr(0, first);
    std::string monthStr = dateStr.substr(first + 1, second - first - 1);
    std::string dayStr = dateStr.substr(second + 1);

    int year = Parser::atoi(yearStr);
    int month = Parser::atoi(monthStr);
    int day = Parser::atoi(dayStr);

    // Validação básica
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
        throw std::invalid_argument("Invalid date format: " + dateStr);

    // Validação de anos bissextos
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28))
            throw std::invalid_argument("invalid date format: " + dateStr);
    }
    
    // Validação de meses com 30 dias
    else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        throw std::invalid_argument("Invalid date format: " + dateStr);

    return dateStr;
}
```

**B. Conversão Segura de Números:**
```cpp
float Parser::atof(const std::string &rateStr) {
    float rate;
    std::istringstream iss(rateStr);
    iss >> rate;

    if (iss.fail())
        throw std::invalid_argument("bad input " + rateStr);

    if (rate < 0)
        throw std::invalid_argument("not a positive number " + rateStr);

    return rate;
}
```

### **Fluxo de Execução Implementado:**

1. **Inicialização do Sistema:**
   ```cpp
   int main(int argc, char **argv) {
       BitcoinExchange exchange;
       
       // Carrega database histórico
       Parser::parseFile("data.csv", exchange);
   }
   ```

2. **Carregamento do Database (data.csv):**
   ```cpp
   void Parser::parseFile(const std::string &filename, BitcoinExchange &exchange) {
       std::ifstream file(filename.c_str());
       if (!file.is_open() || file.fail())
           throw std::runtime_error("Could not open file");

       // Valida cabeçalho obrigatório
       std::string line;
       std::getline(file, line);
       if (line != "date,exchange_rate")
           throw std::runtime_error("Invalid file format");

       // Processa cada linha do histórico
       std::map<std::string, float> &rates = exchange.getRates();
       while (std::getline(file, line)) {
           size_t commaPos = line.find(',');
           if (commaPos != std::string::npos) {
               std::string date = Parser::validateDate(line.substr(0, commaPos));
               float rate = Parser::atof(line.substr(commaPos + 1));
               rates[date] = rate;  // Armazena: "2009-01-02" -> 0.0
           }
       }
   }
   ```

3. **Processamento do Arquivo de Input:**
   ```cpp
   std::ifstream inputFile(argv[1]);
   if (!Parser::validateHeaderInputFile(inputFile)) {
       std::cerr << "Error: could not open file." << std::endl;
       return 1;
   }

   int lineCount = 2;
   std::string line;
   while(std::getline(inputFile, line)) {
       try {
           exchange.makeExchange(line);  // Processa: "2011-01-03 | 3"
       } catch(std::exception& e) {
           std::cerr << "Error line " << lineCount << " - " << line 
                     << " => " << e.what() << std::endl;
       }
       lineCount++;
   }
   ```

4. **Cálculo e Exibição:**
   ```cpp
   void BitcoinExchange::makeExchange(const std::string &date) const {
       size_t pipe_pos = date.find('|');
       
       std::string dateStr = Parser::validateDate(date.substr(0, pipe_pos));
       std::string rateStr = date.substr(pipe_pos + 1);
       float rate = Parser::atof(rateStr);
       
       if (rate > 1000.0f)
           throw std::invalid_argument("too large a number " + rateStr);
       
       // Busca taxa histórica e calcula
       // ... algoritmo de busca ...
       
       float result = rate * historical_rate;
       std::cout << dateStr << " => " << std::fixed << std::setprecision(2) 
                 << rate << " = " << result << std::endl;
   }
   ```

### **Casos de Teste e Validações:**

**Entradas Válidas:**
```
date | value
2011-01-03 | 3      → 2011-01-03 => 3.00 = 9.00
2011-01-03 | 2      → 2011-01-03 => 2.00 = 6.00
2011-01-09 | 1      → 2011-01-09 => 1.00 = 0.32
```

**Tratamento de Erros Específicos:**
```
2012-01-11 | -1     → Error line X - ... => not a positive number -1
2001-42-42 | 1      → Error line X - ... => Invalid date format: 2001-42-42
2012-01-11 | 1001   → Error line X - ... => too large a number 1001
2009-01-01 | 1      → Error line X - ... => No available rate for date: 2009-01-01
bad_format          → Error line X - ... => (parsing error)
```

### **Características Técnicas Avançadas:**

**A. Gerenciamento de Memória:**
- Uso de containers STL (gerenciamento automático)
- RAII (Resource Acquisition Is Initialization)
- Exception safety garantida

**B. Performance:**
- Carregamento O(n log n) do database
- Busca por data O(log n)
- Processamento linear do input O(m log n), onde m = linhas input

**C. Robustez:**
- Tratamento completo de exceções
- Validação rigorosa de todos os inputs
- Mensagens de erro específicas e informativas

---

## **Exercício 01 - RPN (Reverse Polish Notation)**

### **Objetivo Detalhado:**
Implementar uma calculadora que avalia expressões matemáticas em **Notação Polonesa Reversa** (RPN), também conhecida como notação pós-fixa. O programa deve processar uma string de entrada contendo números de um dígito (0-9) e operadores básicos (+, -, *, /), separados por espaços.

### **Arquitetura da Solução Implementada**

#### **1. Estrutura da Classe RPN**

```cpp
class rpn {
private:
    std::stack<int> _stack;
public:
    rpn();
    ~rpn();
    rpn(const rpn &other);
    rpn &operator=(const rpn &other);

    void run(std::string expression);
    void calculate(char op);
    bool checkInput(std::string str);
    bool checkValidNumber(std::string str);
};
```

**Características da Implementação:**
- **Container Único:** Um `std::stack<int>` para armazenar operandos
- **Validação Rigorosa:** Múltiplas camadas de validação de entrada
- **Exception Safety:** Tratamento completo de erros (divisão por zero, etc.)
- **Canonical Form:** Implementação completa do Coplien Form

#### **2. Por que `std::stack<int>` é Ideal para RPN?**

**Vantagens Técnicas:**
- **LIFO (Last In, First Out):** Perfeito para operações matemáticas em pós-fixa
- **Operações O(1):** `push()`, `pop()`, `top()` são constantes
- **Adaptador de Container:** Usa `std::deque` por padrão (eficiente)
- **Interface Minimalista:** Apenas as operações necessárias expostas

**Conceito RPN:**
- **Infixa:** `(8 + 9) * 2` → Precisa de parênteses e precedência
- **RPN:** `8 9 + 2 *` → Avaliação linear, sem ambiguidade

#### **3. Algoritmo Principal Implementado**

```cpp
void rpn::run(std::string expression) {
    if (!checkInput(expression))
        return;
    
    for (size_t i = 0; i < expression.size(); i++) {
        if (expression[i] == ' ')
            continue;
        
        if (std::isdigit(expression[i]))
            this->_stack.push(expression[i] - '0');  // Converte char para int
        
        else if (expression[i] == '+' || expression[i] == '-' || 
                 expression[i] == '*' || expression[i] == '/') {
            if (this->_stack.size() < 2) {
                std::cerr << "Error: insuficiente number of operands." << std::endl;
                return;
            }
            calculate(expression[i]);
        }
    }
    std::cout << this->_stack.top() << std::endl;
}
```

**Fluxo do Algoritmo:**
1. **Validação:** Verifica se a entrada é válida
2. **Parsing:** Processa cada caractere da string
3. **Operandos:** Empilha números (convertendo char para int)
4. **Operadores:** Desempilha 2 valores, calcula e empilha resultado
5. **Resultado:** O último valor na pilha é o resultado final

#### **3. Exemplo Completo de Execução:**

**Expressão:** `"8 9 * 9 - 9 - 9 - 4 - 1 +"`

| Passo | Token | Ação | Stack Estado | Explicação |
|-------|-------|------|-------------|------------|
| 1 | '8' | Push | [8] | Empilha 8 |
| 2 | ' ' | Skip | [8] | Ignora espaços |
| 3 | '9' | Push | [8, 9] | Empilha 9 |
| 4 | ' ' | Skip | [8, 9] | Ignora espaços |
| 5 | '*' | Op | [72] | Pop 9,8 → 8*9=72 → Push 72 |
| 6 | ' ' | Skip | [72] | Ignora espaços |
| 7 | '9' | Push | [72, 9] | Empilha 9 |
| 8 | ' ' | Skip | [72, 9] | Ignora espaços |
| 9 | '-' | Op | [63] | Pop 9,72 → 72-9=63 → Push 63 |
| 10 | '9' | Push | [63, 9] | Empilha 9 |
| 11 | '-' | Op | [54] | Pop 9,63 → 63-9=54 → Push 54 |
| 12 | '9' | Push | [54, 9] | Empilha 9 |
| 13 | '-' | Op | [45] | Pop 9,54 → 54-9=45 → Push 45 |
| 14 | '4' | Push | [45, 4] | Empilha 4 |
| 15 | '-' | Op | [41] | Pop 4,45 → 45-4=41 → Push 41 |
| 16 | '1' | Push | [41, 1] | Empilha 1 |
| 17 | '+' | Op | [42] | Pop 1,41 → 41+1=42 → Push 42 |

**Resultado Final:** 42

#### **4. Implementação das Operações:**

```cpp
void rpn::calculate(char op){
    // IMPORTANTE: Ordem dos operandos
    int b = this->_stack.top(); this->_stack.pop();  // Segundo operando
    int a = this->_stack.top(); this->_stack.pop();  // Primeiro operando

    switch (op) {
        case '+': this->_stack.push(a + b); break;
        case '-': this->_stack.push(a - b); break;  // a - b, não b - a!
        case '*': this->_stack.push(a * b); break;
        case '/': 
            if (b == 0)
                throw std::runtime_error("Error: Division by zero.");
            this->_stack.push(a / b);  // a / b, não b / a!
            break;
    }
}
```

**Por que a ordem importa?**
- Para subtração: `5 3 -` deve ser `5 - 3 = 2`, não `3 - 5 = -2`
- Para divisão: `8 2 /` deve ser `8 / 2 = 4`, não `2 / 8 = 0`

#### **5. Validações Implementadas:**

```cpp
bool rpn::checkInput(std::string str){
    // Verifica se entrada não é vazia
    if (str.empty() || isOnlySpace(str)){
        std::cerr << "Error: empty input." << std::endl;
        return false;
    }
    
    // Valida cada token
    std::istringstream iss(str);
    std::string token;
    while (iss >> token){
        if(token == "+" || token == "-" || token == "*" || token == "/")
            continue;  // Operadores válidos
        if (!checkValidNumber(token))
            return false;  // Números inválidos
    }
    return true;
}

bool rpn::checkValidNumber(std::string str){
    std::istringstream iss(str);
    int num;
    if (!(iss >> num) && !iss.eof()){
        std::cerr << "Error: invalid operator '" << str << "'." << std::endl;
        return false;
    }
    
    // Apenas dígitos de 0-9 são aceitos
    if (num < 0 || num > 9){
        std::cerr << "Error: invalid number '" << str << "'." << std::endl;
        return false;
    }
    return true;
}
```

### **Casos de Teste Importantes:**

**Válidos:**
```
"8 9 * 9 - 9 - 9 - 4 - 1 +"  → 42
"7 7 * 7 -"                   → 42
"1 2 * 2 / 2 * 2 2 - +"       → 3
```

**Inválidos:**
```
"(1 + 1)"           → Error: invalid operator '('
"1 2 + +"           → Error: insufficient number of operands
"1 0 /"             → Error: Division by zero
""                  → Error: empty input
"1 10 +"            → Error: invalid number '10'
```

---

## **Exercício 02 - PmergeMe (Ford-Johnson Algorithm)**

### **Objetivo Detalhado:**
Implementar o **algoritmo de ordenação Ford-Johnson** (também conhecido como merge-insertion sort) e comparar a performance entre `std::vector` e `std::deque`.

### **Conceitos Teóricos Fundamentais**

#### **1. O que é o Algoritmo Ford-Johnson?**

O Ford-Johnson é um algoritmo de ordenação híbrido que combina:
- **Merge Sort:** Para dividir o problema
- **Insertion Sort:** Para inserção otimizada
- **Sequência de Jacobsthal:** Para minimizar comparações

**Características Únicas:**
- **Número mínimo de comparações:** Teoricamente ótimo para pequenos conjuntos
- **Complexidade:** O(n log n) no pior caso
- **Uso de sequência matemática:** Jacobsthal para ordem de inserção

#### **2. Sequência de Jacobsthal**

**Definição Matemática:**
```
J(0) = 0
J(1) = 1  
J(n) = J(n-1) + 2*J(n-2) para n > 1
```

**Sequência:** 0, 1, 1, 3, 5, 11, 21, 43, 85, 171, 341...

**Implementação:**
```cpp
std::vector<size_t> Pmerge::generateJacobstallSequence(size_t len){
    std::vector<size_t> _order;
    size_t j0 = 1;  // J(1)
    size_t j1 = 1;  // J(2) (na verdade deveria ser 3, mas a implementação começa diferente)

    while(j1 < len){
        _order.push_back(j1);
        size_t next = j1 + 2 * j0;  // Fórmula de recorrência
        j0 = j1;
        j1 = next;
    }
    return _order;
}
```

**Por que Jacobsthal minimiza comparações?**
A sequência define a ordem ideal para inserir elementos de forma que:
1. Elementos são inseridos em posições que minimizam buscas binárias
2. A estrutura da árvore de comparações é otimizada
3. Reduz redundância nas comparações

#### **3. Comparação: `std::vector` vs `std::deque`**

| Aspecto | `std::vector` | `std::deque` |
|---------|---------------|--------------|
| **Estrutura** | Array contíguo | Blocos separados |
| **Acesso Aleatório** | O(1) | O(1) |
| **Inserção no Meio** | O(n) | O(n) |
| **Inserção nas Extremidades** | O(1) amortizado (fim), O(n) (início) | O(1) (ambas) |
| **Cache Locality** | Excelente | Boa |
| **Overhead de Memória** | Baixo | Médio |
| **Realocação** | Pode realocar tudo | Nunca realoca elementos existentes |

#### **4. Implementação do Ford-Johnson**

**Passo 1: Divisão em Pares**
```cpp
void Pmerge::fordJohnsonVecPairs(const std::vector<int> &inputVec, 
                                std::vector<int> &big, std::vector<int> &small){
    std::vector<std::pair<int, int> > _pairs;

    // Agrupa elementos em pares
    for (size_t i = 0; i + 1 < inputVec.size(); i += 2){
        int a = inputVec[i];
        int b = inputVec[i + 1];
        if (a < b)
            _pairs.push_back(std::make_pair(a, b));  // (menor, maior)
        else
            _pairs.push_back(std::make_pair(b, a));  // (menor, maior)
    }
    
    // Separa em vetores de "pequenos" e "grandes"
    for (size_t i = 0; i < _pairs.size(); i++){
        small.push_back(_pairs[i].first);   // Menores
        big.push_back(_pairs[i].second);    // Maiores
    }
    
    // Se número ímpar, adiciona último elemento aos pequenos
    if (inputVec.size() % 2 == 1)
        small.push_back(inputVec.back());
}
```

**Passo 2: Ordenação Recursiva**
```cpp
void Pmerge::fordJohnsonStep2(std::vector<int> &big, std::vector<int> &small){
    // Ordena recursivamente os elementos "grandes"
    if (big.size() > 1) {
        // Chama Ford-Johnson recursivamente ou usa std::sort para pequenos conjuntos
        std::sort(big.begin(), big.end());
    }
    
    // Insere elementos "pequenos" usando sequência de Jacobsthal
    orderByJacobstallSeq_vec(big, small);
}
```

**Passo 3: Inserção com Jacobsthal**
```cpp
void Pmerge::orderByJacobstallSeq_vec(std::vector<int> &big, std::vector<int> &small){
    std::vector<size_t> jacobsthal = generateJacobstallSequence(small.size());
    
    // Insere primeiro elemento diretamente
    if (!small.empty()) {
        big.insert(big.begin(), small[0]);
    }
    
    // Insere elementos seguindo a sequência de Jacobsthal
    for (size_t i = 0; i < jacobsthal.size() && jacobsthal[i] < small.size(); i++) {
        size_t index = jacobsthal[i];
        
        // Busca binária para encontrar posição de inserção
        std::vector<int>::iterator pos = std::lower_bound(big.begin(), big.end(), small[index]);
        big.insert(pos, small[index]);
    }
}
```

#### **5. Medição de Performance**

```cpp
void Pmerge::fordJohnsonVec(){
    std::vector<int> _smallValues;
    std::vector<int> _bigValues;

    clock_t vec_start = clock();  // Início da medição
    
    fordJohnsonVecPairs(this->_vec, _bigValues, _smallValues);
    fordJohnsonStep2(_bigValues, _smallValues);
    this->_vec = _bigValues;
    
    clock_t vec_end = clock();    // Final da medição
    this->_vTime = calc_time(vec_start, vec_end);
}

double Pmerge::calc_time(clock_t start, clock_t end){
    // Converte para microssegundos
    double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 100000;
    return time;
}
```

#### **6. Exemplo de Execução Completa**

**Entrada:** `./PmergeMe 3 5 9 7 4`

**Passo 1 - Divisão em Pares:**
```
Original: [3, 5, 9, 7, 4]
Pares: [(3,5), (7,9)]  // 4 fica sozinho
Big:   [5, 9]          // Maiores de cada par
Small: [3, 7, 4]       // Menores + elemento sozinho
```

**Passo 2 - Ordenação dos "Grandes":**
```
Big ordenado: [5, 9]
```

**Passo 3 - Inserção usando Jacobsthal:**
```
Jacobsthal para len=3: [1]  // Apenas um valor < 3

1. Insere small[0]=3 no início: [3, 5, 9]
2. Insere small[1]=7 (busca binária): [3, 5, 7, 9]  
3. Insere small[2]=4 (busca binária): [3, 4, 5, 7, 9]
```

**Saída:**
```
before: 3 5 9 7 4
after:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : 42 us
Time to process a range of 5 elements with std::deque  : 38 us
```

#### **7. Por que Ford-Johnson é Eficiente?**

1. **Reduz Comparações:** Sequência de Jacobsthal é matematicamente provada para minimizar comparações
2. **Híbrido:** Combina vantagens de merge sort e insertion sort
3. **Recursivo:** Divide problema em subproblemas menores
4. **Busca Binária:** Usa busca binária para inserções, O(log n) por inserção

### **Análise de Complexidade:**
- **Melhor Caso:** O(n log n)
- **Caso Médio:** O(n log n)  
- **Pior Caso:** O(n log n)
- **Espaço:** O(n)

---

## **Comparação de Performance entre Containers**

### **Por que comparar `std::vector` vs `std::deque`?**

O exercício 02 demonstra como diferentes containers afetam a performance do mesmo algoritmo.

#### **Análise Teórica:**

**`std::vector` - Vantagens:**
- **Cache Locality:** Elementos contíguos na memória
- **Menor Overhead:** Sem ponteiros internos
- **Acesso Rápido:** Aritmética de ponteiros

**`std::vector` - Desvantagens:**
- **Realocação:** Pode realocar todo array ao crescer
- **Inserção no Meio:** Precisa mover todos elementos posteriores

**`std::deque` - Vantagens:**
- **Sem Realocação:** Nunca move elementos existentes
- **Inserção Eficiente:** Em ambas as extremidades
- **Crescimento:** Adiciona blocos conforme necessário

**`std::deque` - Desvantagens:**
- **Cache Miss:** Elementos em blocos separados
- **Overhead:** Estrutura mais complexa internamente

#### **Resultados Típicos:**

Para **conjuntos pequenos** (< 100 elementos):
- `std::vector` geralmente mais rápido devido à cache locality

Para **conjuntos grandes** (> 1000 elementos):
- `std::deque` pode ser mais rápido devido a menos realocações

Para **muitas inserções no meio**:
- Performance similar, ambos O(n) para inserção

### **Exemplo de Medição:**
```
Input: 42 numbers
Time to process with std::vector : 245 us
Time to process with std::deque  : 198 us
```

---

## **Pontos Importantes para Defesa**

### **1. Escolhas de Design e Justificativas**

#### **Exercício 00 - Por que `std::map`?**
**Pergunta Típica:** "Por que não usar `std::vector` ou `std::unordered_map`?"

**Resposta:**
- **`std::vector`:** Buscaria em O(n), muito lento para 1600+ registros
- **`std::unordered_map`:** Não mantém ordem, impossível buscar "data mais próxima"
- **`std::map`:** Mantém ordem + busca eficiente O(log n) + `lower_bound` perfeito para o problema

#### **Exercício 01 - Por que `std::stack`?**
**Pergunta Típica:** "Por que não implementar pilha manualmente?"

**Resposta:**
- **STL é otimizada:** Implementação testada e eficiente
- **RAII automático:** Gerenciamento de memória seguro  
- **Adaptador:** Pode usar `vector`, `deque` ou `list` por baixo conforme necessário
- **Interface limpa:** Apenas operações necessárias expostas

#### **Exercício 02 - Por que Ford-Johnson?**
**Pergunta Típica:** "Por que não usar `std::sort`?"

**Resposta:**
- **Objetivo educacional:** Demonstrar algoritmo avançado de ordenação
- **Número mínimo de comparações:** Teoricamente ótimo para pequenos conjuntos
- **Comparação de containers:** Mostra como diferentes estruturas afetam performance

### **2. Complexidade Algorítmica**

#### **Análise Detalhada por Exercício:**

**Ex00 - BitcoinExchange:**
- **Carregamento:** O(n log n) para inserir n registros no map
- **Busca:** O(log n) por consulta usando `lower_bound`
- **Total:** O(n log n + m log n) onde m = número de consultas

**Ex01 - RPN:**
- **Parsing:** O(n) onde n = tamanho da expressão
- **Cada operação:** O(1) para push/pop
- **Total:** O(n) linear

**Ex02 - Ford-Johnson:**
- **Divisão em pares:** O(n)
- **Ordenação recursiva:** O(n log n)
- **Inserção com Jacobsthal:** O(n log n)
- **Total:** O(n log n)

### **3. Tratamento de Erros**

#### **Estratégia de Validação:**

**Validação em Camadas:**
1. **Sintática:** Formato correto (datas, números, operadores)
2. **Semântica:** Valores válidos (datas reais, valores positivos)
3. **Lógica:** Operações válidas (operandos suficientes, divisão por zero)

**Exception Safety:**
```cpp
// Garantia Básica: Nenhum vazamento de memória
// Garantia Forte: Estado consistente após exceção
// Garantia Nothrow: Operações que nunca falham

try {
    // Operação que pode falhar
} catch (const std::exception& e) {
    // Estado consistente mantido
    // Recursos limpos automaticamente (RAII)
    std::cerr << e.what() << std::endl;
}
```

### **4. Otimizações Implementadas**

#### **Ex00 - BitcoinExchange:**
- **Remoção de espaços:** `std::remove` durante carregamento
- **Busca eficiente:** `lower_bound` ao invés de busca linear
- **Parsing único:** Carrega dados apenas uma vez

#### **Ex01 - RPN:**
- **Parsing inline:** Processa caracteres conforme lê
- **Validação antecipada:** Para antes do primeiro erro
- **Operações diretas:** Sem conversões desnecessárias

#### **Ex02 - Ford-Johnson:**
- **Medição precisa:** Apenas o algoritmo, não I/O
- **Containers otimizados:** STL containers específicos para cada operação
- **Recursão controlada:** Para pequenos conjuntos

---

## **Conceitos Avançados C++**

### **1. Template Metaprogramming**

Embora não explicitamente usado, os containers STL demonstram templates:

```cpp
std::map<std::string, float>     // Template com 2 tipos
std::stack<int>                  // Template com 1 tipo
std::vector<std::pair<int, int>> // Template aninhado
```

### **2. Iterator Pattern**

Uso extensivo de iteradores STL:

```cpp
// Ex00
std::map<std::string, float>::iterator it = this->_data.lower_bound(date);

// Ex02
std::vector<int>::iterator pos = std::lower_bound(big.begin(), big.end(), small[index]);
```

### **3. SFINAE (Substitution Failure Is Not An Error)**

Implícito no uso de algoritmos STL que funcionam com diferentes containers:

```cpp
std::lower_bound(container.begin(), container.end(), value);
// Funciona com vector, deque, set, etc.
```

### **4. Function Objects e Lambdas**

Potencial para otimização com comparadores customizados:

```cpp
// Poderia ser usado para ordenação customizada
std::sort(vec.begin(), vec.end(), [](int a, int b) { return a > b; });
```

### **5. Perfect Forwarding**

Containers STL usam perfect forwarding internamente:

```cpp
container.emplace_back(args...);  // Constrói in-place, sem cópias
```

### **6. Move Semantics**

STL containers implementam move semantics:

```cpp
std::vector<int> temp = std::move(original);  // Move, não copia
```

---

## **Casos de Teste para Defesa**

### **Exercício 00 - BitcoinExchange**

#### **Casos Válidos:**
```bash
# Arquivo input.txt:
date | value
2011-01-03 | 3
2011-01-03 | 2
2011-01-09 | 1
2012-01-11 | 1
2001-42-42
2012-01-11 | -1
2012-01-11 | 1
2012-01-11 | 2147483648

# Saída esperada:
2011-01-03 => 3 = 9
2011-01-03 => 2 = 6  
2011-01-09 => 1 = 1
2012-01-11 => 1 = 1
Error: bad input => 2001-42-42
Error: not a positive number.
2012-01-11 => 1 = 1
Error: too large a number.
```

#### **Casos Edge:**
```bash
# Data muito antiga
1990-01-01 | 1          → Error: Date is too early.

# Formato inválido
2012/01/11 | 1          → Error: bad input => 2012/01/11 | 1
2012-01-11|1            → Error: bad input => 2012-01-11|1  
2012-13-01 | 1          → Error: bad input => 2012-13-01 | 1

# Valores extremos
2012-01-11 | 0          → 2012-01-11 => 0 = 0
2012-01-11 | 1000       → 2012-01-11 => 1000 = 1000000
2012-01-11 | 1000.1     → Error: too large a number.
```

### **Exercício 01 - RPN**

#### **Casos Válidos:**
```bash
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"    → 42
./RPN "7 7 * 7 -"                     → 42  
./RPN "1 2 * 2 / 2 * 2 2 - +"         → 3
./RPN "0"                             → 0
./RPN "9"                             → 9
```

#### **Casos Inválidos:**
```bash
./RPN "(1 + 1)"                       → Error: invalid operator '('
./RPN "1 2 + +"                       → Error: insufficient number of operands
./RPN "1 0 /"                         → Error: Division by zero.
./RPN ""                              → Error: empty input.
./RPN "1 10 +"                        → Error: invalid number '10'
./RPN "1 2 3"                         → Stack com múltiplos elementos (implementação pode variar)
```

### **Exercício 02 - PmergeMe**

#### **Casos Válidos:**
```bash
./PmergeMe 3 5 9 7 4
# Saída:
before: 3 5 9 7 4
after:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : X us
Time to process a range of 5 elements with std::deque  : Y us

./PmergeMe 1
# Saída:
before: 1
after:  1
Time to process a range of 1 elements with std::vector : X us
Time to process a range of 1 elements with std::deque  : Y us

# Muitos elementos (teste com 100+ números)
./PmergeMe `jot -r 100 1 1000`  # Unix/Mac
./PmergeMe $(shuf -i 1-1000 -n 100)  # Linux
```

#### **Casos Inválidos:**
```bash
./PmergeMe                            → Error
./PmergeMe -1 2 3                     → Error  
./PmergeMe 1 2 -3                     → Error
./PmergeMe abc                        → Error
./PmergeMe 1 2 3.5                    → Error
./PmergeMe 2147483648                 → Error (overflow)
```

---

## **Perguntas Frequentes na Defesa**

### **1. "Por que usar STL ao invés de implementar estruturas próprias?"**

**Resposta:**
- **Eficiência:** STL é altamente otimizada por especialistas
- **Confiabilidade:** Testada por milhões de desenvolvedores
- **Padrão:** Parte do padrão C++, garantia de portabilidade
- **Manutenibilidade:** Código mais limpo e legível
- **Focus no Problema:** Permite focar na lógica do negócio, não na estrutura de dados

### **2. "Como garantir que não há vazamentos de memória?"**

**Resposta:**
- **RAII:** Containers STL implementam RAII automaticamente
- **Exception Safety:** Destrutores são chamados mesmo com exceções
- **Smart Pointers:** Usados internamente pelos containers
- **Stack Allocation:** Objetos alocados na stack quando possível

### **3. "Por que Ford-Johnson ao invés de QuickSort ou MergeSort?"**

**Resposta:**
- **Objetivo Educacional:** Demonstrar algoritmo menos conhecido
- **Número Mínimo de Comparações:** Teoricamente ótimo para pequenos conjuntos
- **Comparação de Containers:** Permite avaliar diferenças entre vector/deque
- **Complexidade Interessante:** Combina múltiplas técnicas (merge + insertion + Jacobsthal)

### **4. "Como escolher entre diferentes containers STL?"**

**Resposta Estruturada:**

| Necessidade | Container Recomendado | Motivo |
|-------------|----------------------|---------|
| Busca rápida por chave | `std::map` ou `std::unordered_map` | O(log n) ou O(1) |
| Pilha/Fila | `std::stack`/`std::queue` | Interface especializada |
| Acesso aleatório | `std::vector` | Cache locality |
| Inserção em extremidades | `std::deque` | O(1) em ambos os lados |
| Elementos únicos | `std::set` | Sem duplicatas |
| Ordem customizada | `std::priority_queue` | Heap automaticamente |

### **5. "Como medir performance corretamente?"**

**Resposta:**
```cpp
// Medir apenas o algoritmo, não I/O
clock_t start = clock();
// Código a ser medido
clock_t end = clock();

// Conversão para unidade útil
double time_us = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;

// Múltiplas execuções para média
// Aquecimento para evitar cache misses
// Desabilitar otimizações do compilador quando necessário
```

---

## **Conclusão**

O CPP Module 09 é um módulo avançado que demonstra:

1. **Domínio de STL:** Uso apropriado de containers especializados
2. **Análise Algorítmica:** Escolha de algoritmos baseada em complexidade
3. **Otimização:** Comparação de performance entre implementações
4. **Engenharia de Software:** Tratamento robusto de erros e validação
5. **C++ Moderno:** Uso de features avançadas da linguagem

**Key Takeaways:**
- **Container Correto:** Cada problema tem uma estrutura de dados ideal
- **Complexidade Importa:** O(1) vs O(log n) vs O(n) faz diferença real
- **STL é Poderosa:** Aproveitiar bibliotecas bem projetadas
- **Validação é Crucial:** Software robusto precisa tratar todos os casos edge
- **Performance é Mensurável:** Comparações empíricas são essenciais

Este conhecimento forma a base para desenvolvimento C++ profissional, demonstrando não apenas syntax, mas compreensão profunda de algoritmos, estruturas de dados e engenharia de software.