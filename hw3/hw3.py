import argparse


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


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Perform multiple pairwise sequence alignments using the Needleman-Wunsch algorithm.")
    parser.add_argument('--input', '-i', type=str, required=True, help="Input file containing paths to FASTA files.")
    parser.add_argument('--output', '-o', type=str, required=True,
                        help="Output file path for PHYLIP formatted alignments.")
    parser.add_argument('--gap', type=int, required=True, help="Gap penalty.")
    parser.add_argument('--match', type=int, required=True, help="Matching score.")
    parser.add_argument('--mismatch', type=int, required=True, help="Mismatch penalty.")

    return parser.parse_args()


def read_paths(input_file):
    paths = []
    try:
        with open(input_file, 'r') as file:
            paths = [line.strip() for line in file if line.strip()]
    except Exception as e:
        print(f"An error occurred while reading the input file: {e}")
    return paths


def read_fasta(file_path):
    header, sequence = None, ''
    try:
        with open(file_path, 'r') as file:
            header_found = False
            for line in file:
                if line.startswith('>'):
                    if header_found:  # If more than one header is found, break and use the first one.
                        break
                    header = line.strip().split('>')[1]
                    header_found = True
                elif header_found:
                    sequence += line.strip()
        if not header:  # No header found in the file
            header = "Unknown"
    except IOError as e:
        print(f"An error occurred while reading the FASTA file {file_path}: {e}")
        header, sequence = "Error", ""  # In case of reading error
    return header, sequence


"""
Writes the aligned sequences to an output file in PHYLIP format.
"""
def write_phylip_file(aligned_sequences, output_file):

    # Convert dict_items to a list if necessary or directly use it in a loop
    aligned_sequences = list(aligned_sequences)

    with open(output_file, 'w') as f:
        # PHYLIP format starts with the number of sequences and the length of the sequences
        num_sequences = len(aligned_sequences)
        sequence_length = max(len(seq) for name, seq in aligned_sequences)

        f.write(f"{num_sequences} {sequence_length}\n")
        for name, seq in aligned_sequences:
            # PHYLIP format requires the sequence name to be exactly 10 characters long, padded if necessary
            f.write(f"{name[:10].ljust(10)} {seq}\n")


def progressive_alignment(sequences, match_score, mismatch_penalty, gap_penalty):
    if not sequences:
        return []

    # Start the alignment with the first sequence
    current_alignment = [sequences[0]]  # Encapsulate the first sequence in a list to align progressively

    # Align each subsequent sequence against the progressively aligned sequences
    for new_seq in sequences[1:]:
        new_alignment = []
        # Align the new sequence against all previously aligned sequences
        for aligned_seq in current_alignment:
            # Align the new sequence against each of the aligned sequences in current_alignment
            aligned_seq1, aligned_seq_new = needleman_wunsch(aligned_seq, new_seq, match_score, mismatch_penalty,
                                                             gap_penalty)
            new_alignment.append(aligned_seq1)  # Store the new version of the aligned sequence
        new_alignment.append(aligned_seq_new)  # Also add the newly aligned sequence to the new alignment
        current_alignment = new_alignment  # Update current_alignment with new_alignment

    return current_alignment


def main():
    args = parse_arguments()

    # Read sequences from FASTA files listed in the input file
    sequences = []
    with open(args.input, 'r') as f:
        for file_name in f:
            header, sequence = read_fasta(file_name.strip())
            sequences.append(sequence)  # Store only sequences for alignment

    # Perform progressive multiple sequence alignment
    aligned_sequences = progressive_alignment(sequences, args.match, args.mismatch, args.gap)

    # Prepare and write the output in PHYLIP format
    write_phylip_file([(f"Sequence_{i + 1}", seq) for i, seq in enumerate(aligned_sequences)], args.output)


if __name__ == "__main__":
    main()
