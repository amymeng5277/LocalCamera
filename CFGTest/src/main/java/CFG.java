import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.MethodNode;

import java.util.*;

public class CFG {
    Set<Node> nodes = new HashSet<Node>();
    Map<Node, Set<Node>> edges = new HashMap<Node, Set<Node>>();

    static class Node {
        int position;
        MethodNode method;
        ClassNode clazz;

        Node(int p, MethodNode m, ClassNode c) {
            position = p;
            method = m;
            clazz = c;
        }

        /**
         * Because I changed the hashCode() method below, so this equals() method should be updated to for consistency in collection operations.
         *
         * @param o Object
         * @return true if equals
         */
        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;

            Node node = (Node) o;

            if (position != node.position) return false;
            if (!method.equals(node.method)) return false;
            return clazz.equals(node.clazz);
        }

        /**
         * Actually the former hashCode() is not very good practice, so I change it to be <b>coupled with prime number 31</b>.
         *
         * @return integer
         */
        @Override
        public int hashCode() {
            int result = position;
            result = 31 * result + method.hashCode();
            result = 31 * result + clazz.hashCode();
            return result;
        }

        public String toString() {
            return clazz.name + "." +
                    method.name + method.signature + ": " + position;
        }
    }

    private void addNodeNotExist(Node n) {
        if (!this.nodes.contains(n))
            this.nodes.add(n);
    }

    public void addNode(int p, MethodNode m, ClassNode c) {
        Node n = new Node(p, m, c);
        addNodeNotExist(n);
    }

    private void addEdgeN12N2(Node n1, Node n2) {
        if (!this.edges.containsKey(n1))
            this.edges.put(n1, new HashSet<Node>());
        this.edges.get(n1).add(n2);
    }

    public void addEdge(int p1, MethodNode m1, ClassNode c1,
                        int p2, MethodNode m2, ClassNode c2) {
        Node n1 = new Node(p1, m1, c1);
        Node n2 = new Node(p2, m2, c2);
        addNodeNotExist(n1);
        addNodeNotExist(n2);
        addEdgeN12N2(n1, n2);
    }

    public void deleteNode(int p, MethodNode m, ClassNode c) {
        Node n = new Node(p, m, c);
        boolean deleteSuccess = this.nodes.remove(n);
        if (deleteSuccess) { // means node exists
            this.edges.remove(n); // delete edges
            for (Map.Entry<Node, Set<Node>> e : this.edges.entrySet()) {
                if (e.getValue().contains(n))
                    e.getValue().remove(n);
            }
        }
    }

    private void deleteNodeIfIsolated(Node n) {
        boolean isolated = true;
        for (Map.Entry<Node, Set<Node>> e : this.edges.entrySet()) {
            if (e.getValue().contains(n)) { // some other node points to node n1
                isolated = false;
                break;
            }
        }
        if (isolated) {
            this.nodes.remove(n);
        }
    }

    public void deleteEdge(int p1, MethodNode m1, ClassNode c1,
                           int p2, MethodNode m2, ClassNode c2) {
        Node n1 = new Node(p1, m1, c1);
        Node n2 = new Node(p2, m2, c2);
        if (this.nodes.contains(n1)
                && this.nodes.contains(n2)
                && this.edges.containsKey(n1)) { // ensure node n1 & node n2 & edge n1->n2 exist
            this.edges.get(n1).remove(n2);

            deleteNodeIfIsolated(n1);
            deleteNodeIfIsolated(n2);
        }
    }


    public boolean isReachable(int p1, MethodNode m1, ClassNode c1,
                               int p2, MethodNode m2, ClassNode c2) {
        Node n1 = new Node(p1, m1, c1);
        Node n2 = new Node(p2, m2, c2);
        Deque<Node> queue = new ArrayDeque<Node>();
        queue.add(n1);
        while (!queue.isEmpty()) {
            Node firstNode = queue.pollFirst();
            if (firstNode.equals(n2)) {
                return true;
            }
            Set<Node> nodes = this.edges.get(firstNode);
            if (nodes != null && nodes.size() != 0) {
                for (Node n : nodes) {
                    queue.add(n);
                }
            }
        }
        return false;
    }
}
