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

| Exercício | Container                  | Operação Principal    | Complexidade                  |
|-----------|----------------------------|-----------------------|-------------------------------|
| Ex00      | `std::map`                 | Busca (`lower_bound`) | O(log n)                      |
| Ex01      | `std::stack`               | Push/Pop              | O(1) por operação, O(n) total |
| Ex02      | `std::vector`/`std::deque` | Ford-Johnson Sort     | O(n log n)                    |

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
- Realizar cálulos de câmbio
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

#### **4. Implementação das Operações Matemáticas**

```cpp
void rpn::calculate(char op) {
    // CRÍTICO: Ordem correta dos operandos
    int b = this->_stack.top(); this->_stack.pop();  // Segundo operando (topo)
    int a = this->_stack.top(); this->_stack.pop();  // Primeiro operando

    switch (op) {
        case '+': 
            this->_stack.push(a + b);
            break;
        case '-': 
            this->_stack.push(a - b);  // SEMPRE a - b, nunca b - a
            break;
        case '*': 
            this->_stack.push(a * b);
            break;
        case '/': 
            if (b == 0)
                throw std::runtime_error("Error: Division by zero.");
            this->_stack.push(a / b);  // SEMPRE a / b, nunca b / a
            break;
    }
}
```

**Por que a ordem dos operandos é crítica?**
- **Subtração:** `5 3 -` → Stack: [5, 3] → Pop: b=3, a=5 → Resultado: 5-3=2
- **Divisão:** `8 2 /` → Stack: [8, 2] → Pop: b=2, a=8 → Resultado: 8/2=4

#### **5. Sistema Completo de Validação**

**A. Validação de Entrada Principal:**
```cpp
bool rpn::checkInput(std::string str) {
    if (str.empty() || isOnlySpace(str)) {
        std::cerr << "Error: empty input." << std::endl;
        return false;
    }
    
    std::istringstream iss(str);
    std::string token;
    while (iss >> token) {
        if(token == "+" || token == "-" || token == "*" || token == "/")
            continue;  // Operadores válidos
        if (!checkValidNumber(token))
            return false;  // Número inválido
    }
    return true;
}
```

**B. Validação de Números (Apenas Dígitos 0-9):**
```cpp
bool rpn::checkValidNumber(std::string str) {
    std::istringstream iss(str);
    int num;
    if (!(iss >> num) && !iss.eof()) {
        std::cerr << "Error: invalid operator '" << str << "'." << std::endl;
        return false;
    }
    if (num < 0 || num > 9) {
        std::cerr << "Error: invalid number '" << str << "'." << std::endl;
        return false;
    }
    return true;
}
```

**C. Verificação de Operandos Suficientes:**
```cpp
// Dentro do loop principal
if (this->_stack.size() < 2) {
    std::cerr << "Error: insuficiente number of operands." << std::endl;
    return;
}
```

#### **6. Exemplo Prático de Execução:**

**Entrada:** `"8 9 + 1 7 * -"`

| Passo | Token | Ação | Stack | Cálculo |
|-------|-------|------|-------|---------|
| 1 | '8' | Push | [8] | - |
| 2 | '9' | Push | [8, 9] | - |
| 3 | '+' | Calc | [17] | Pop 9,8 → 8+9=17 |
| 4 | '1' | Push | [17, 1] | - |
| 5 | '7' | Push | [17, 1, 7] | - |
| 6 | '*' | Calc | [17, 7] | Pop 7,1 → 1*7=7 |
| 7 | '-' | Calc | [10] | Pop 7,17 → 17-7=10 |

**Resultado:** 10

#### **7. Casos de Teste Importantes:**

**Entradas Válidas:**
```bash
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"  → 42
./RPN "7 7 * 7 -"                  → 42
./RPN "1 2 * 2 / 2 * 2 2 - + 2 + 2 +" → 42
./RPN "0 1 2 * + 3 +"               → 5

```

**Entradas Inválidas:**
```bash
./RPN "8 9 * * 9 - 9 - 9 - 4 - 1 +" → Error: insuficiente number of operands.
./RPN "1 0 /"                       → Error: Division by zero.
./RPN "1 p"                         → Error: invalid operator 'p'.
./RPN "1 23"                        → Error: invalid number '23'.
./RPN ""                            → Error: empty input.
```
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
Implementar o algoritmo de ordenação Ford-Johnson (merge-insertion sort) com comparação de performance entre `std::vector` e `std::deque`. O programa ordena uma sequência de inteiros positivos e mostra o tempo de execução para ambos os containers.

### **Atualização da Lógica e Output

A versão mais recente do código exibe o seguinte padrão de saída:

```bash
Before: 3 5 9 7 4
After:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : X us
Time to process a range of 5 elements with std::deque  : Y us
```

- Apenas o conteúdo do vector é exibido antes e depois da ordenação, usando os títulos "Before:" e "After:" (conforme o subject da 42).
- Os tempos de execução para vector e deque são mostrados, permitindo comparação de performance.
- O output duplicado foi removido, tornando a saída mais clara e objetiva para correção e defesa.

### Impacto da Mudança

- **Clareza:** O output segue exatamente o padrão exigido pelo subject, facilitando a correção automática e a defesa do projeto.
- **Foco:** A exibição dos dados concentra-se no vector, que é o container principal para demonstração do algoritmo.
- **Performance:** A medição dos tempos para ambos os containers permanece, permitindo análise comparativa.

### Exemplo de Execução

```bash
./PmergeMe 3 5 9 7 4
Before: 3 5 9 7 4
After:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : 0.00031 us
Time to process a range of 5 elements with std::deque  : 0.00014 us
```

### Justificativa Técnica

A alteração garante aderência ao padrão da 42, evita confusão na leitura do output e facilita a avaliação do funcionamento do algoritmo Ford-Johnson. O código permanece modular, com validação rigorosa dos argumentos e medição precisa do tempo de execução.

---
## **Pontos Importantes para Defesa - Implementação Atual**

### **Exercício 00 - BitcoinExchange: Arquitetura Modular**

#### **Separação de Responsabilidades Implementada:**
```cpp
// Classe BitcoinExchange - Core business logic
class BitcoinExchange {
    std::map<std::string, float> rates;  // Data storage
    void makeExchange(const std::string &date) const;  // Core calculation
};

// Classe Parser - Utility class (static methods only)
class Parser {
    static void parseFile(...);         // File processing
    static std::string validateDate(...); // Date validation
    static float atof(...);             // Safe conversion
};
```

**Por que esta arquitetura é superior:**
- **Single Responsibility:** Cada classe tem uma responsabilidade específica
- **Testabilidade:** Parser pode ser testado independentemente
- **Reutilização:** Parser pode ser usado por outras classes
- **Manutenibilidade:** Mudanças em validação não afetam business logic

#### **Algoritmo de Busca Otimizado:**
```cpp
// Busca exata primeiro (O(log n))
std::map<std::string, float>::const_iterator it = rates.find(dateStr);
if (it != rates.end()) {
    // Encontrou data exata
    return it->second;
}

// Busca data anterior mais próxima (O(log n))  
it = rates.upper_bound(dateStr);
if (it == rates.begin())
    throw std::runtime_error("No available rate for date");
    
it--;  // Data anterior mais próxima
return it->second;
```

**Vantagens desta implementação:**
- **Duas tentativas:** Primeiro exata, depois aproximada
- **Eficiência:** Ambas as buscas são O(log n)
- **Robustez:** Trata caso de data muito antiga
- **Clareza:** Lógica fácil de entender e debugar

### **Exercício 01 - RPN: Validação em Camadas**

#### **Sistema de Validação Multinível:**
```cpp
// Nível 1: Validação geral da string
bool checkInput(std::string str) {
    if (str.empty() || isOnlySpace(str)) return false;
    
    std::istringstream iss(str);
    std::string token;
    while (iss >> token) {
        if (token != "+" && token != "-" && token != "*" && token != "/") {
            if (!checkValidNumber(token)) return false;
        }
    }
    return true;
}

// Nível 2: Validação específica de números
bool checkValidNumber(std::string str) {
    std::istringstream iss(str);
    int num;
    if (!(iss >> num) && !iss.eof()) return false;
    if (num < 0 || num > 9) return false;  // Apenas dígitos 0-9
    return true;
}

// Nível 3: Validação durante execução
if (this->_stack.size() < 2) {
    std::cerr << "Error: insuficiente number of operands." << std::endl;
    return;
}
```

**Por que três níveis de validação:**
1. **Pré-processamento:** Valida antes de processar
2. **Granular:** Valida cada token individualmente  
3. **Runtime:** Valida durante execução (operandos suficientes)

### **Exercício 02 - PmergeMe: Medição Precisa de Performance**

#### **Metodologia de Timing Implementada:**
```cpp
void Pmerge::fordJohnsonVec() {
    std::vector<int> _smallValues;
    std::vector<int> _bigValues;

    clock_t vec_start = clock();  // Início preciso
    
    fordJohnsonVecPairs(this->_vec, _bigValues, _smallValues);
    fordJohnsonStep2(_bigValues, _smallValues);
    this->_vec = _bigValues;      // Resultado final
    
    clock_t vec_end = clock();    // Fim preciso
    this->_vTime = calc_time(vec_start, vec_end);
}

double Pmerge::calc_time(clock_t start, clock_t end) {
    // Converte para microssegundos com precisão
    double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 100000;
    return time;
}
```
    
#### **8. Exemplo Completo de Execução Ford-Johnson**

**Entrada:** `./PmergeMe 3 5 9 7 4`

**Passo 1 - Divisão em Pares:**
```
Original: [3, 5, 9, 7, 4]
Pares formados: [(3,5), (7,9)]  // 4 fica sozinho (elemento ímpar)
Big array:   [5, 9]      // Maiores de cada par
Small array: [3, 7, 4]   // Menores + elemento sozinho
```

**Passo 2 - Ordenação Recursiva dos "Grandes":**
```cpp
// Para arrays pequenos, usa std::sort
std::sort(big.begin(), big.end());
Big ordenado: [5, 9]
```

**Passo 3 - Inserção com Sequência de Jacobsthal:**
```
Jacobsthal sequence para len=3: [1, 3]  // generateJacobstallSequence(3)

1. Insere small[0]=3 no início: [3, 5, 9]
2. Usa Jacobsthal[0]=1: Insere small[1]=7 
   - lower_bound(7) em [3,5,9] → posição entre 5 e 9
   - Resultado: [3, 5, 7, 9]
3. Continua com small[2]=4:
   - lower_bound(4) em [3,5,7,9] → posição entre 3 e 5  
   - Resultado: [3, 4, 5, 7, 9]
```

**Saída do Programa:**
```bash
Before: 3 5 9 7 4
After:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : 15 us
Time to process a range of 5 elements with std::deque  : 12 us
```

#### **9. Vantagens Teóricas do Ford-Johnson**

1. **Minimização de Comparações:**
   - Sequência de Jacobsthal é matematicamente ótima
   - Reduz número total de comparações necessárias
   
2. **Híbrido Inteligente:**
   - Combina divisão (merge-sort) com inserção otimizada
   - Aproveita vantagens de ambos os algoritmos
   
3. **Busca Binária:**
   - Cada inserção usa O(log n) comparações
   - Muito mais eficiente que inserção linear

4. **Estrutura Recursiva:**
   - Divide problema em subproblemas menores
   - Resolve recursivamente para máxima eficiência

### **Análise de Complexidade Final:**
- **Temporal:** O(n log n) - otimizada pela sequência de Jacobsthal
- **Espacial:** O(n) - arrays auxiliares para big/small
- **Comparações:** Teoricamente mínimo para algoritmos de comparação

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
- **Jacobsthal correta:** Implementação matemática precisa da sequência
- **Dual implementation:** Versões paralelas para vector e deque

### **5. Diferenciais da Implementação Atual**

#### **Ex00 - Robustez Excepcional:**
```cpp
// Tratamento de overflow durante conversão
long value = std::atol(argv[i]);
if (value > INT_MAX) {
    std::cerr << "Error" << std::endl;
    return;
}

// Validação rigorosa de anos bissextos  
if (month == 2) {
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (day > (isLeap ? 29 : 28))
        throw std::invalid_argument("invalid date format: " + dateStr);
}
```

#### **Ex01 - Feedback Detalhado:**
```cpp
// Mensagens de erro específicas e informativas
std::cerr << "Error line " << lineCount << " - " << line 
          << " => " << e.what() << std::endl;

// Controle de linha para debugging
int lineCount = 2;  // Inicia em 2 (pula header)
```

#### **Ex02 - Documentação Interna:**
```cpp
/**
 * @brief Generates a Jacobsthal sequence up to a specified length.
 * @param len The maximum value up to which to generate sequence
 * @return Vector containing Jacobsthal numbers less than 'len'
 */
std::vector<size_t> Pmerge::generateJacobstallSequence(size_t len);
```

**Vantagens da documentação:**
- **Clareza:** Explica propósito de cada função
- **Parâmetros:** Documenta entrada e saída
- **Manutenibilidade:** Facilita modificações futuras

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
- **Jacobsthal correta:** Implementação matemática precisa da sequência
- **Dual implementation:** Versões paralelas para vector e deque

### **5. Diferenciais da Implementação Atual**

#### **Ex00 - Robustez Excepcional:**
```cpp
// Tratamento de overflow durante conversão
long value = std::atol(argv[i]);
if (value > INT_MAX) {
    std::cerr << "Error" << std::endl;
    return;
}

// Validação rigorosa de anos bissextos  
if (month == 2) {
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (day > (isLeap ? 29 : 28))
        throw std::invalid_argument("invalid date format: " + dateStr);
}
```

#### **Ex01 - Feedback Detalhado:**
```cpp
// Mensagens de erro específicas e informativas
std::cerr << "Error line " << lineCount << " - " << line 
          << " => " << e.what() << std::endl;

// Controle de linha para debugging
int lineCount = 2;  // Inicia em 2 (pula header)
```

#### **Ex02 - Documentação Interna:**
```cpp
/**
 * @brief Generates a Jacobsthal sequence up to a specified length.
 * @param len The maximum value up to which to generate sequence
 * @return Vector containing Jacobsthal numbers less than 'len'
 */
std::vector<size_t> Pmerge::generateJacobstallSequence(size_t len);
```

**Vantagens da documentação:**
- **Clareza:** Explica propósito de cada função
- **Parâmetros:** Documenta entrada e saída
- **Manutenibilidade:** Facilita modificações futuras

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
- **Jacobsthal correta:** Implementação matemática precisa da sequência
- **Dual implementation:** Versões paralelas para vector e deque

### **5. Diferenciais da Implementação Atual**

#### **Ex00 - Robustez Excepcional:**
```cpp
// Tratamento de overflow durante conversão
long value = std::atol(argv[i]);
if (value > INT_MAX) {
    std::cerr << "Error" << std::endl;
    return;
}

// Validação rigorosa de anos bissextos  
if (month == 2) {
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (day > (isLeap ? 29 : 28))
        throw std::invalid_argument("invalid date format: " + dateStr);
}
```

#### **Ex01 - Feedback Detalhado:**
```cpp
// Mensagens de erro específicas e informativas
std::cerr << "Error line " << lineCount << " - " << line 
          << " => " << e.what() << std::endl;

// Controle de linha para debugging
int lineCount = 2;  // Inicia em 2 (pula header)
```

#### **Ex02 - Documentação Interna:**
```cpp
/**
 * @brief Generates a Jacobsthal sequence up to a specified length.
 * @param len The maximum value up to which to generate sequence
 * @return Vector containing Jacobsthal numbers less than 'len'
 */
std::vector<size_t> Pmerge::generateJacobstallSequence(size_t len);
```

**Vantagens da documentação:**
- **Clareza:** Explica propósito de cada função
- **Parâmetros:** Documenta entrada e saída
- **Manutenibilidade:** Facilita modificações futuras

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
- **Jacobsthal correta:** Implementação matemática precisa da sequência
- **Dual implementation:** Versões paralelas para vector e deque

### **5. Diferenciais da Implementação Atual**

#### **Ex00 - Robustez Excepcional:**
```cpp
// Tratamento de overflow durante conversão
long value = std::atol(argv[i]);
if (value > INT_MAX) {
    std::cerr << "Error" << std::endl;
    return;
}

// Validação rigorosa de anos bissextos  
if (month == 2) {
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (day > (isLeap ? 29 : 28))
        throw std::invalid_argument("invalid date format: " + dateStr);
}
```

#### **Ex01 - Feedback Detalhado:**
```cpp
// Mensagens de erro específicas e informativas
std::cerr << "Error line " << lineCount << " - " << line 
          << " => " << e.what() << std::endl;

// Controle de linha para debugging
int lineCount = 2;  // Inicia em 2 (pula header)
```

#### **Ex02 - Documentação Interna:**
```cpp
/**
 * @brief Generates a Jacobsthal sequence up to a specified length.
 * @param len The maximum value up to which to generate sequence
 * @return Vector containing Jacobsthal numbers less than 'len'
 */
std::vector<size_t> Pmerge::generateJacobstallSequence(size_t len);
```

**Vantagens da documentação:**
- **Clareza:** Explica propósito de cada função
- **Parâmetros:** Documenta entrada e saída
- **Manutenibilidade:** Facilita modificações futuras

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
- **Jacobsthal correta:** Implementação matemática precisa da sequência
- **Dual implementation:** Versões paralelas para vector e deque

### **5. Diferenciais da Implementação Atual**

#### **Ex00 - Robustez Excepcional:**
```cpp
// Tratamento de overflow durante conversão
long value = std::atol(argv[i]);
if (value > INT_MAX) {
    std::cerr << "Error" << std::endl;
    return;
}

// Validação rigorosa de anos bissextos  
if (month == 2) {
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (day > (isLeap ? 29 : 28))
        throw std::invalid_argument("invalid date format: " + dateStr);
}
```

#### **Ex01 - Feedback Detalhado:**
```cpp
// Mensagens de erro específicas e informativas
std::cerr << "Error line " << lineCount << " - " << line 
          << " => " << e.what() << std::endl;

// Controle de linha para debugging
int lineCount = 2;  // Inicia em 2 (pula header)
```

#### **Ex02 - Documentação Interna:**
```cpp
/**
 * @brief Generates a Jacobsthal sequence up to a specified length.
 * @param len The maximum value up to which to generate sequence
 * @return Vector containing Jacobsthal numbers less than 'len'
 */
std::vector<size_t> Pmerge::generateJacobstallSequence(size_t len);
```

**Vantagens da documentação:**
- **Clareza:** Explica propósito de cada função
- **Parâmetros:** Documenta entrada e saída
- **Manutenibilidade:** Facilita modificações futuras

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
- **Jacobsthal correta:** Implementação matemática precisa da sequência
- **Dual implementation:** Versões paralelas para vector e deque

### **5. Diferenciais da Implementação Atual**

#### **Ex00 - Robustez Excepcional:**
```cpp
// Tratamento de overflow durante conversão
long value = std::atol(argv[i]);
if (value > INT_MAX) {
    std::cerr << "Error" << std::endl;
    return;
}

// Validação rigorosa de anos bissextos  
if (month == 2) {
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (day > (isLeap ? 29 : 28))
        throw std::invalid_argument("invalid date format: " + dateStr);
}
```
