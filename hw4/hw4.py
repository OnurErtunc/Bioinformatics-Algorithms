import argparse
from Bio import SeqIO
import csv
from Bio import Phylo
from io import StringIO
import matplotlib.pyplot as plt


def parse_args():
    parser = argparse.ArgumentParser(description="UPGMA Phylogenetic Tree Construction")
    parser.add_argument("--fasta", type=str, help="Path to the FASTA file containing all sequences.")
    parser.add_argument("--gapopen", type=int, help="Gap opening penalty")
    parser.add_argument("--gapext", type=int, help="Gap extension penalty")
    parser.add_argument("--match", type=int, help="Match score")
    parser.add_argument("--mismatch", type=int, help="Mismatch penalty")
    parser.add_argument("--outtree", type=str, help="Output file for Newick tree")
    parser.add_argument("--outmat", type=str, help="Output file for distance or similarity matrix")
    parser.add_argument("--outplot", type=str, help="Output file for the tree plot")
    return parser.parse_args()


def calculate_distance_matrix(sequences, match_score, mismatch_penalty, gap_penalty):
    num_sequences = len(sequences)
    distance_matrix = [[0] * num_sequences for _ in range(num_sequences)]

    for i in range(num_sequences):
        for j in range(i + 1, num_sequences):
            aligned_seq1, aligned_seq2 = needleman_wunsch(sequences[i], sequences[j], match_score, mismatch_penalty, gap_penalty)
            distance_matrix[i][j] = distance_matrix[j][i] = calculate_edit_distance(aligned_seq1, aligned_seq2)

    return distance_matrix

def calculate_edit_distance(seq1, seq2):
    # Simple edit distance calculation based on the aligned sequences
    return sum(1 for a, b in zip(seq1, seq2) if a != b) + abs(len(seq1) - len(seq2))


def needleman_wunsch(seq1, seq2, match_score, mismatch_penalty, gap_penalty):
    # Initialize the scoring matrix
    rows, cols = len(seq1) + 1, len(seq2) + 1
    score_matrix = [[0 for _ in range(cols)] for _ in range(rows)]

    # Initialize the traceback matrix
    traceback_matrix = [[None for _ in range(cols)] for _ in range(rows)]

    # Fill the first row and column of the matrix with gap penalties
    for i in range(1, rows):
        score_matrix[i][0] = i * gap_penalty
        traceback_matrix[i][0] = 'up'
    for j in range(1, cols):
        score_matrix[0][j] = j * gap_penalty
        traceback_matrix[0][j] = 'left'

    # Fill in the rest of the scoring matrix
    for i in range(1, rows):
        for j in range(1, cols):
            match = score_matrix[i - 1][j - 1] + (match_score if seq1[i - 1] == seq2[j - 1] else mismatch_penalty)
            delete = score_matrix[i - 1][j] + gap_penalty
            insert = score_matrix[i][j - 1] + gap_penalty
            score_matrix[i][j] = max(match, delete, insert)

            # Record the traceback path
            if score_matrix[i][j] == match:
                traceback_matrix[i][j] = 'diag'
            elif score_matrix[i][j] == delete:
                traceback_matrix[i][j] = 'up'
            else:
                traceback_matrix[i][j] = 'left'

    # Traceback to find the optimal alignment
    aligned_seq1, aligned_seq2 = [], []
    i, j = rows - 1, cols - 1
    while i > 0 or j > 0:
        if traceback_matrix[i][j] == 'diag':
            aligned_seq1.append(seq1[i - 1])
            aligned_seq2.append(seq2[j - 1])
            i -= 1
            j -= 1
        elif traceback_matrix[i][j] == 'up':
            aligned_seq1.append(seq1[i - 1])
            aligned_seq2.append('-')
            i -= 1
        else:  # left
            aligned_seq1.append('-')
            aligned_seq2.append(seq2[j - 1])
            j -= 1

    # Reverse the aligned sequences as we've built them from the end to the start
    return ''.join(reversed(aligned_seq1)), ''.join(reversed(aligned_seq2))


class Cluster:
    def __init__(self, label, index):
        self.label = label  # The label of the cluster (initially the sequence label)
        self.index = index  # The index in the original distance matrix
        self.distance = 0  # Distance to parent node in the tree
        self.children = []

def upgma(distance_matrix, labels):
    clusters = [Cluster(labels[i], i) for i in range(len(labels))]
    queue = clusters[:]

    while len(queue) > 1:
        # Find the pair of clusters with the smallest distance
        min_distance = float('inf')
        pair_to_merge = (None, None)
        for i in range(len(queue)):
            for j in range(i + 1, len(queue)):
                if distance_matrix[queue[i].index][queue[j].index] < min_distance:
                    min_distance = distance_matrix[queue[i].index][queue[j].index]
                    pair_to_merge = (i, j)

        # Merge the two closest clusters
        cluster1, cluster2 = queue[pair_to_merge[0]], queue[pair_to_merge[1]]
        new_label = f"({cluster1.label},{cluster2.label})"
        new_cluster = Cluster(new_label, len(clusters))
        new_cluster.distance = min_distance / 2
        new_cluster.children = [cluster1, cluster2]
        clusters.append(new_cluster)

        # Calculate new distances for the new cluster
        new_distances = []
        for i in range(len(queue)):
            if queue[i] not in (cluster1, cluster2):
                dist1 = distance_matrix[queue[i].index][cluster1.index]
                dist2 = distance_matrix[queue[i].index][cluster2.index]
                new_distances.append((dist1 + dist2) / 2)

        # Reconstruct the queue and distance matrix
        new_queue = [c for c in queue if c not in (cluster1, cluster2)] + [new_cluster]
        new_distance_matrix = []
        for i in range(len(new_queue) - 1):  # Exclude the new cluster temporarily
            new_row = [distance_matrix[new_queue[i].index][new_queue[j].index] for j in range(len(new_queue) - 1)]
            new_row.append(new_distances[i])  # Append distance to the new cluster
            new_distance_matrix.append(new_row)

        # Append the last row for the new cluster to the matrix
        new_distances.append(0)  # Distance to itself is zero
        new_distance_matrix.append(new_distances)

        # Update the distance matrix and the queue
        distance_matrix = new_distance_matrix
        queue = new_queue
        for i, cluster in enumerate(queue):
            cluster.index = i  # Reassign indices based on the new queue

    return clusters[-1]  # Return the root of the tree



def print_newick(cluster, newick_str=''):
    # Recursive function to build Newick format string
    if cluster.children:
        newick_str += '('
        newick_str = print_newick(cluster.children[0], newick_str)
        newick_str += ','
        newick_str = print_newick(cluster.children[1], newick_str)
        newick_str += f'){cluster.label}:{cluster.distance}'
    else:
        newick_str += f'{cluster.label}:{cluster.distance}'
    return newick_str

def write_distance_matrix(matrix, labels, file_path):
    with open(file_path, 'w', newline='') as f:
        writer = csv.writer(f, delimiter='\t')  # Change delimiter to ',' for CSV
        writer.writerow([''] + labels)  # Header row
        for label, row in zip(labels, matrix):
            writer.writerow([label] + row)


def plot_tree(newick_str, file_path):
    handle = StringIO(newick_str)
    tree = Phylo.read(handle, "newick")
    Phylo.draw(tree)
    tree.ladderize()  # Optional: Ladderize the tree for better visualization
    tree.root_at_midpoint()  # Optional: Root the tree at its midpoint
    Phylo.draw(tree, do_show=False)
    plt.savefig(file_path)

def main():
    args = parse_args()
    sequences = list(SeqIO.parse(args.fasta, "fasta"))
    labels = [seq.id for seq in sequences]
    sequence_strs = [str(seq.seq) for seq in sequences]

    # Calculate distance matrix and output
    distance_matrix = calculate_distance_matrix(sequence_strs, args.match, args.mismatch, args.gapopen)
    write_distance_matrix(distance_matrix, labels, args.outmat)

    # Construct the tree and output Newick format
    root_cluster = upgma(distance_matrix, labels)
    newick_str = print_newick(root_cluster) + ';'
    print("Newick Format Tree:")
    print(newick_str)
    with open(args.outtree, 'w') as f:
        f.write(newick_str)

    # Plot the tree
    plot_tree(newick_str, args.outplot)

if __name__ == "__main__":
    main()
