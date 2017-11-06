import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.io.PrintStream;

/**
 * Created by mengdongqi on 16-01-28.
 */

public class TestM {

    private M objectM;
    private OutputStream os;
    private PrintStream ps;


    public TestM() {
        os = new ByteArrayOutputStream();
        ps = new PrintStream(os);
    }

    @Before
    public void setup() {
        objectM = new M();
        System.setOut(ps);
    }

    @After
    public void tearDown() {
        objectM = null;
        System.setOut(System.out);
    }

    /**
     * (1) Node coverage but not edge coverage
     * Nodes: 1,2,3,4,8,10,11
     * Edges: [1,2] [2,3] [3,4] [4,8] [8,10] [10,11]
     * Edge Pairs: [1,2,3] [2,3,4] [3,4,8] [4,8,10] [8,10,11]
     * Prime Path: [1,2,3,4,8,10,11]
     */
    @Test
    public void testM_Node_Coverage_1() {
        objectM.m("", 0);
        Assert.assertEquals("zero" + System.getProperty("line.separator"), os.toString());
    }

    /**
     * Nodes: 1,2,3,5,8,9,11
     * ∗ Additional Nodes: 5,9
     * ∗ Edges: [1,2] [2,3] [3,5] [5,8] [8,9] [9,11]
     * ∗ Edge Pairs:[1,2,3] [2,3,5] [3,5,8] [5,8,9] [8,9,11]
     * ∗ Prime Path: [1,2,3,5,8,9,11]
     */
    @Test
    public void testM_Node_Coverage_2() {
        objectM.m("A", 0);
        Assert.assertEquals("a" + System.getProperty("line.separator"), os.toString());
    }

    /**
     * Nodes: 1,2,3,6,7,8,9,11
     * ∗ Additional Nodes: 6,7
     * ∗ Edges: [1,2] [2,3] [3,6] [6,7] [7,8] [8,9] [9,11]
     * ∗ Edge Pairs: [1,2,3] [2,3,6] [3,6,7] [6,7,8] [7,8,9] [8,9,11]
     * ∗ Prime Path: [1,2,3,5,8,9,11]
     */
    @Test
    public void testM_Node_Coverage_3() {
        objectM.m("Am", 0);
        Assert.assertEquals("b" + System.getProperty("line.separator"), os.toString());
    }

    /**
     * (2) Edge coverage but not edge-pair coverage;
     * Edges: [1,3] [3,7] [7,8] [8,9] [9,11]
     * Additional Edges: [1,3] [3,7]
     * Edge Pairs: [1,3,7] [3,7,8] [7,8,9] [8,9,11]
     * Additional Edge Pairs: [1,3,7] [3,7,8]
     * Prime Path: [1,3,7,8,9,11]
     */
    @Test
    public void testM_Edge_Coverge_1() {
        objectM.m("Amy", 1);
        Assert.assertEquals("b" + System.getProperty("line.separator"), os.toString());
    }

    /**
     * (4) Edge-pair coverage and prime path coverage
     * Edge Pairs: [1,3,4] [3,4,8] [4,8,10] [8,10,11]
     * Additional Edge Pairs: [1,3,4]
     * Prime Path: [1,3,4,8,10,11]
     */

    @Test
    public void testM_EdgePair_PrimPath_Coverage_1() {
        objectM.m("", 1);
        Assert.assertEquals("zero" + System.getProperty("line.separator"), os.toString());
    }

    /**
     * Edge Pairs: [1,3,5] [3,5,8] [5,8,9 [8,9,11]
     * Additional Edge Pairs: [1,3,5]
     * Prime Path: [1,3,5,8,9,11]
     */
    @Test
    public void testM_EdgePair_PrimPath_Coverage_2() {
        objectM.m("A", 1);
        Assert.assertEquals("a" + System.getProperty("line.separator"), os.toString());
    }

    /**
     * Edge Pairs: [1,3,6] [3,6,7] [6,7,8] [7,8,9] [8,9,11]
     * Additional Edge Pairs: [1,3,6]
     * Prime Path: [1,3,6,7,8,9,11]
     */
    @Test
    public void testM_EdgePair_PrimPath_Coverage_3() {
        objectM.m("Am", 1);
        Assert.assertEquals("b" + System.getProperty("line.separator"), os.toString());
    }

    /**
     * Edge Pairs: [1,2,3] [2,3,7] [3,7,8] [7,8,9] [8,9,11]
     * Additional Edge Pairs: [2,3,7]
     * Prime Path: [1,2,3,7,8,9,11]
     */
    @Test
    public void testM_EdgePair_PrimPath_Coverage_4() {
        objectM.m("Amy", 0);
        Assert.assertEquals("b" + System.getProperty("line.separator"), os.toString());
    }
}

class M {
    public static void main(String[] argv) {
        M obj = new M();
        if (argv.length > 0)
            obj.m(argv[0], argv.length);
    }

    public void m(String arg, int i) {
        int q = 1;
        A o = null;
        Impossible nothing = new Impossible();
        if (i == 0)
            q = 4;
        q++;
        switch (arg.length()) {
            case 0:
                q /= 2;
                break;
            case 1:
                o = new A();
                new B();
                q = 25;
                break;
            case 2:
                o = new A();
                q = q * 100;
            default:
                o = new B();
                break;
        }
        if (arg.length() > 0) {
            o.m();
        } else {
            System.out.println("zero");
        }
        nothing.happened();
    }
}

class A {
    public void m() {
        System.out.println("a");
    }
}

class B extends A {
    public void m() {
        System.out.println("b");
    }
}

class Impossible {
    public void happened() {
        // "2b||!2b?", whatever the answer nothing happens here
    }
}
