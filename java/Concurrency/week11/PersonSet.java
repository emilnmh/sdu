import java.util.Set;
import java.util.HashSet;

public class PersonSet {

    public static void main() {
        PersonSet pSet = new PersonSet();
        Person p1 = new Person("P1");
        Person p2 = new Person("P2");
        Person p3 = new Person("P3");
        Person p4 = new Person("P4");
        Person p5 = new Person("P5");
        Person p6 = new Person("P6");
        Person p7 = new Person("P7");
        Person p8 = new Person("P8");
        Person p9 = new Person("P9");
        Person p10 = new Person("P10");
        
        pSet.addPerson(p1);
        pSet.addPerson(p2);
        pSet.addPerson(p3);
        pSet.addPerson(p4);

        Thread t1 = new Thread(() -> {
            Set<Person> set = pSet.mySet();
            set.add(p5);
            set.add(p6);
            set.add(p7);
            set.add(p8);
            set.add(p9);
            set.add(p10);
        });

        Thread t2 = new Thread(() -> {
            boolean contains = pSet.containsPerson(p10);
            System.out.println(contains);
        });

        System.out.println(pSet);
        t1.start();
        t2.start();
        
        try {
            t1.join();
            t2.join();
            System.out.println(pSet);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private final Set<Person> mySet = new HashSet<Person>();

    public synchronized void addPerson(Person p) {
        mySet.add(p);
    }

    public synchronized boolean containsPerson(Person p) {
        return mySet.contains(p);
    }

    public synchronized Set<Person> mySet() {
        return mySet;
    }

    public String toString() {
        return mySet.toString();
    }
}