import os
import time


def generate_input():
    insert_num = [100, 50000, 200000]
    query_num = [100, 50000, 300000]
    os.system("clang++ -std=c++11 generator.cpp -o generator")
    for i, (insert, query) in enumerate(zip(insert_num, query_num)):
        os.system(f"./generator ../results/test{i + 1}.in {insert} {query}")


def generate_answer():
    for i in range(3):
        data_dict = {}
        with open(f"../results/test{i + 1}.in", "r") as f, open(
            f"../results/test{i + 1}.ans", "w"
        ) as g:
            for line in f:
                parts = line.split()
                if parts[0] == "0":
                    data_dict[parts[1]] = parts[2]
                elif parts[0] == "1":
                    g.write(data_dict.get(parts[1], "-1") + "\n")


def grade():
    hashing_strategy = ["uneven", "even"]
    collision_strategy = ["linear", "double", "overflow"]
    passed_num = 0

    os.system("clang++ -std=c++11 main.cpp hashtable.cpp -o main")
    for i in range(3):
        print(f"Testing test{i + 1}.in ...")
        for hs in hashing_strategy:
            for cs in collision_strategy:
                start = time.time()
                os.system(
                    f"./main {hs} {cs} < ../results/test{i + 1}.in > ../results/test{i + 1}_{hs}_{cs}.out"
                )
                end = time.time()
                print(f"   Time for {hs} {cs}: {end - start:.3f} s")

                outs = [
                    line.strip()
                    for line in open(f"../results/test{i + 1}_{hs}_{cs}.out")
                ]
                ans = [line.strip() for line in open(f"../results/test{i + 1}.ans")]
                if outs == ans:
                    print(f"   Passed!")
                    passed_num += 1
                else:
                    print(f"   Failed!")
                print("")
        print("")
    print(f"Done! {passed_num}/18 passed!")


if __name__ == "__main__":
    if not os.path.exists("../results"):
        os.makedirs("../results")
    generate_input()
    generate_answer()
    grade()
