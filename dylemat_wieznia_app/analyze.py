import json
import os
console_rows, console_columns = os.popen('stty size', 'r').read().split()
# from pprint import pprint

with open('data.json') as data_file:    
    najlepsze_osobniki = json.load(data_file)


def suma_jedynek(t):
  if not isinstance(t, list): raise Exception("list should be given")
  
  if len(t) > 1:
    return t[0]+suma_jedynek(t[1:])
  else:
    return t[0]

def bit_to_char(bit):
  if bit == 0: return 'W';
  elif bit == 1: return 'Z';
  else: return str(bit)

def convert_to_str(content):
  if isinstance(content, list):
      return '[' + '|'.join(map(bit_to_char, content)) + ']'
  else:
    return content

for osobnik in najlepsze_osobniki:
  for pole in osobnik:
    print(pole, convert_to_str(osobnik[pole]), sep=': ')
  print("suma_sypania/zdrad", suma_jedynek(osobnik["Chromosom"]), sep=': ')
  print("-"*int(console_columns))

for osobnik in najlepsze_osobniki:
  print("id:", str(osobnik["Osobnik"]).ljust(7), ": ", str(osobnik["Sredni_wyrok"]).rjust(3), "lat -> ", convert_to_str(osobnik["Chromosom"]), sep='')