# cpp-datastructures
Uma biblioteca educacional moderna em C++ contendo diversas estruturas de dados, como listas, pilhas, filas, filas de prioridade, grafos e matrizes.

Perfeita para ser reutilizada em projetos acadêmicos através de **submódulos** ou **subtrees Git**.

---

## 📦 Como utilizar a biblioteca

Você pode integrar a biblioteca no seu projeto de duas formas:
    
---

<details>
<summary>🔗 Opção 1: Git Submodule (recomendado)</summary>
<br>
O submódulo permite adicionar esta biblioteca como um "sub-repositório" vinculado a um commit específico.

#### ➕ Adicionando ao seu projeto

```bash
git submodule add https://github.com/LucasGualtieri/cpp-datastructures
git submodule update --init --recursive
```

Isso criará a pasta cpp-datastructures/ com os arquivos da biblioteca.

### 🧠 Vantagens:
- Você mantém o repositório da lib separado e versionado.
- Pode facilmente atualizar a lib com:

```bash
cd cpp-datastructures
git pull origin main
```
</details>

---

<details>
<summary>🌳 Opção 2: Git Subtree (alternativa mais simples)</summary>
<br>
Com o subtree, o conteúdo do outro repositório é mesclado diretamente na árvore do seu projeto.

#### ➕ Adicionando a lib como subtree

```bash
git subtree add --prefix=cpp-datastructures https://github.com/LucasGualtieri/cpp-datastructures main --squash
```

#### 🔄 Atualizando a subtree
```bash
git subtree pull --prefix=cpp-datastructures https://github.com/LucasGualtieri/cpp-datastructures main --squash
```

### 🧠 Vantagens:
- Não exige conhecimento de submódulos para quem clona o repositório.
- Tudo está no mesmo repositório, sem dependências externas.

### ⚠️ Desvantagens:
- Histórico da lib não é preservado.
- Atualizações precisam ser feitas manualmente via comando subtree pull.
</details>

---

### 🛠️ Usando no CMake

```cmake
add_subdirectory(cpp-datastructures)
target_link_libraries(seu_projeto PRIVATE ds)
```
---

### 💻 incluindo no código
```cpp
#include "cpp-datastructures/list/linear_list.hpp"

using namespace std;
using namespace ds;

int main() {

    LinearList<int> list = { 1, 2, 3, 4, 5 };
    cout << list << endl;

    return 0;
}
```

⚙️ Sobre estrutura
- Os headers ficam em ``cpp-datastructures/estrutura/``, então você sempre inclui com ``#include "cpp-datastructures/estrutura/estrutura.hpp"``
<!-- - Não é necessário referenciar a pasta ``include/`` nos ``#include``, pois ela é tratada como raiz via CMake ou -I -->
- <details>
    <summary>📂 File Tree</summary>
    
    ```bash
    .
    ├── LICENSE
    ├── README.md
    ├── graph
    │   ├── data_structure.hpp
    │   ├── edge.hpp
    │   ├── graph.hpp
    │   ├── graph_builder.hpp
    │   └── implementations
    │       ├── adjacency_list.hpp
    │       ├── adjacency_matrix.hpp
    │       ├── adjacency_matrix_pointers.hpp
    │       ├── adjacency_tree.hpp
    │       └── fast_adjacency_list.hpp
    ├── list
    │   ├── doubly_linkedList.hpp
    │   ├── linear_list.hpp
    │   ├── linked_list.hpp
    │   └── list.hpp
    ├── matrix
    │   ├── matrix.hpp
    │   └── matrixInt.hpp
    ├── queue
    │   ├── linear_queue.hpp
    │   ├── linked_queue.hpp
    │   ├── max_heap.hpp
    │   ├── min_heap.hpp
    │   └── queue.hpp
    ├── stack
    │   ├── linear_stack.hpp
    │   ├── linked_stack.hpp
    │   └── stack.hpp
    ├── tests
    │   ├── test_AVL.cpp
    │   ├── test_adjacency_list.cpp
    │   ├── test_adjacency_matrix.cpp
    │   ├── test_doubly_linked_list.cpp
    │   ├── test_fast_adjacency_list.cpp
    │   ├── test_linear_list.cpp
    │   ├── test_linear_queue.cpp
    │   ├── test_linear_stack.cpp
    │   ├── test_linked_list.cpp
    │   ├── test_linked_queue.cpp
    │   ├── test_linked_stack.cpp
    │   ├── test_matrix.cpp
    │   ├── test_matrix_int.cpp
    │   └── test_min_heap.cpp
    ├── tree
    │   ├── AVL.hpp
    │   └── node.hpp
    └── utils
        ├── cell.hpp
        ├── pair.hpp
        ├── timer.hpp
        ├── triple.hpp
        ├── union_find.hpp
        └── util.hpp
    ```
    </details>
