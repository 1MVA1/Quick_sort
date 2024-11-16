import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("../../Quick_sort/results.csv")

# Строим график
plt.figure(figsize=(20, 12))

plt.plot(data['size'], data['random_insertion_sort_times'], label='Random vector insertion sort', marker='.', color='r')
plt.plot(data['size'], data['random_quick_sort_times'], label='Random vector quick sort', marker='.', color='b')
plt.plot(data['size'], data['reverse_insertion_sort_times'], label='Reverse vector insertion sort', marker='.', color='y')
plt.plot(data['size'], data['reverse_quick_sort_times'], label='Reverse vector quick sort', marker='.', color='g')

# Настройки графика
plt.title("Threshold search for improved quick sort")
plt.xlabel("size")
plt.ylabel("time")
plt.legend()

# Показать сетку
plt.grid(True)

plt.savefig("result.png") 

plt.show()
