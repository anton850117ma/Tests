// Concept
// 1. focus on requires expressions
// 2. form: requires (parameter list) {requirement sequence}
// 3. parameter list is optional
// 4. 4 types of requirements
//    (1) simple requirment
//    (2) type requirements
//    (3) compound requirements
//    (4) nested requirements

// Simple: check whether expressions that will compile
template<typename T>
concept Incrementable = requires(T x) { ++x; x++; }; 

// using the concept
template<Incrementable T> void Foo(T t);
template<typename T> requires Incrementable<T> void Foo(T t);
template<typename T> void Foo(T t) requires Incrementable<T>;
void Foo(Incrementable auto t); //should check whether auto is needed

// Type: check named types with keyword "typename"
template<typename T> using Ref = T&;
template<typename T> concept C = requires
{
    typename T::inner; // required nested member name
    typename S<T>;     // required class template specialization
    typename Ref<T>;   // required alias template substitution
};

// TODO: using the above concept in practice


// Compound: { expression } noexcept return-type-requirement;
// noexcept and return-type-requirement are optional
// Example: a conecpt that
//  requires a non-throwing swap() method, and
//  requires a size() method returning a size_t
template<typename T>
concept C = requires (T &x, T &y){
  { x.swap(y) } noexcept;
  { x.size() } -> std::convertible_to<std::size_t>;
};

// Example: from cpluscplus
template<typename T> concept C2 = requires(T x)
{
    // the expression *x must be valid
    // AND the type T::inner must be valid
    // AND the result of *x must be convertible to T::inner
    {*x} -> std::convertible_to<typename T::inner>;
 
    // the expression x + 1 must be valid
    // AND std::same_as<decltype((x + 1)), int> must be satisfied
    // i.e., (x + 1) must be a prvalue of type int
    {x + 1} -> std::same_as<int>;
 
    // the expression x * 1 must be valid
    // AND its result must be convertible to T
    {x * 1} -> std::convertible_to<T>;
};

// Nested: specify additional constraints in terms of local parameters
template<class T>
concept Semiregular = requires(T a, size_t n)
{  
    requires Same<T*, decltype(&a)>; // nested: "Same<...> evaluates to true"
    requires Same<T*, decltype(new T)>; // nested: "Same<...> evaluates to true"
    requires Same<T*, decltype(new T[n])>; // nested
};

// combining concepts:
template<typename T> requires Incrementable<T> && Decrementable<T>
void Foo(T t);

template<typename T> 
concept C = requires Incrementable<T> || Decrementable<T>
void Foo(C auto t);

// Compared to methods before c++20
// 1. easier to write constraints of custom templates

template<typename T>
struct Incrementable_impl {
  template<typename U>
  static auto test(U &&u) -> decltype(++u, u++, std::true_type {});
  static auto test(...) -> std::false_type;
  using type = decltype(test(std::declval<T>()));
}
template<typename T>
struct Incrementable: Incrementable_impl<T>::type {};

// using this check
template<typename T>
void do_increment_twice(T &&t) {
  if constexpr (Incrementable<T>::value) { ++t; t++; }
}

// VS concept

template<typename T>
concept Incrementable = requires(T x) { ++x; x++; };

// using this check
template<Incrementable T> 
void do_increment_twice(T &&t) { ++t; t++; }


// 2. easier to read template error messages
void Foo (Incrementable auto t) { /* ... */ }
class Bar {};
int main() {
  Bar b;
  Foo(b);
}

// TODO: error messages (in practice)


//--------------------------------------

// Lambda expression changes
// before c++20, [=] campture 'this' implicitly
// since c++200, you need to be implicit, so: [=, this]

// Templated Lambda Expressions
// use familiar template syntax with lambda expressions
[] <typename T> (T t) {/*...*/};
[] <typename T, int N> (T (&t)[N]) {/*...*/};

// Motivation 1 (want to know T of vector<T>)
// pre c++20
[] (const auto &vec) {
    using V = std::decay_t<decltype(vec)>;
    using T = typename V::value_type;
    T x {};
    ...
};
// with c++20
[] <typename T> (const vector<T> &vec) {
    T x {};
    ...
};

// Motivation 2 (perfect forwarding)
// pre c++20
[] (auto&& ...args) { return foo(std::forward<decltype(args)>(args)...); };
// with c++20
[] <typename ...T> (T&& ...args) { foo(std::forward<T>(args)...); };

// Pack Expansion in Lambda Captures
// Pre c++20
// A simple-capture followed be an ellipsis is a pack expansion => ok
template<class F, class... Args>
auto delay_invoke(F f, Args ...args) {
  return [f, args...]() -> decltype(auto) { return std::invoke(f, args...); }
}
// An init-capture followed by an ellipsis is ill-formed (well-formed in c++20)
template<class F, class... Args>
auto delay_invoke(F f, Args ...args) {
  return [f = std::move(f), args = std::move(args)...]() -> decltype(auto) { 
    return std::invoke(f, args...); 
   }
}
// use the alternative in pre c++20
template<class F, class... Args>
auto delay_invoke(F f, Args ...args) {
  return [f=std::move(f), tup=std::make_tuple(std::move(args)...)]() -> decltype(auto) {
    return std::apply(f, tup);
  };
}

//--------------------------------------

// constexpr & consteval & constinit

// constexpr changes
// constexpr virtual functions
// constexpr function can now:
// (1) use dynamic_cast and typeid
// (2) do dynamic memory allocations, global new/delete
// (3) must be transient constexpr allocations
// (4) contain try/catch blocks
//     * but still cannot throw exceptions
// std::string and std::vector are now constexpr
// TODO: give examples in practice

// Immediate function - consteval
// constexpr function
//  might be called at compile time, but not a hard requirement

constexpr auto InchToMm(double inch) { return inch * 25.4; }

const double const_inch { 6 };
const auto mm1 { InchToMm(const_inch) };   // at compile time

double dynamic_inch { 8 };
const auto mm2 { InchToMm(dynamic_inch) };  // at run time

// Consteval function
//  required to always produce a constant at compile time, a non-constant result should be a compilation error
//  implies inline as constexpr
//  called: immediate functions

consteval auto InchToMm(double inch) { return inch * 25.4; }

const double const_inch { 6 };
const auto mm1 { InchToMm(const_inch) };   // Fine, everything is constant

double dynamic_inch { 8 };
const auto mm2 { InchToMm(dynamic_inch) }; // Compilation error: not constant

// Constinit
// asserts that a variable has static initialization, i.e. zero initialization and constant initialization, 
//  otherwise the program is ill-formed
// declares variable with static or thread storage duration, e.g. static and thread_local
// cannot be used with constexpr and consteval
// when the declared variable is a reference, constinit is equivalent to constexpr
// can be used with object whcih has constexpr constructors and no constexpr destructor
// prevent the static initialization order fiasco

constinit const char *a = { /*...*/ };

// TODO: example of static initialization order fiasco

//--------------------------------------









