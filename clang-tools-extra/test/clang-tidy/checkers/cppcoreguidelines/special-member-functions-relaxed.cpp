// RUN: %check_clang_tidy %s cppcoreguidelines-special-member-functions %t -- -config="{CheckOptions: {cppcoreguidelines-special-member-functions.AllowMissingMoveFunctions: true, cppcoreguidelines-special-member-functions.AllowSoleDefaultDtor: true, cppcoreguidelines-special-member-functions.AllowImplicitlyDeletedCopyOrMove: true}}" --

// Don't warn on destructors without definitions, they might be defaulted in another TU.
class DeclaresDestructor {
  ~DeclaresDestructor();
};

class DefinesDestructor {
  ~DefinesDestructor();
};
DefinesDestructor::~DefinesDestructor() {}
// CHECK-MESSAGES: [[@LINE-4]]:7: warning: class 'DefinesDestructor' defines a non-default destructor but does not define a copy constructor or a copy assignment operator [cppcoreguidelines-special-member-functions]

class DefinesDefaultedDestructor {
  ~DefinesDefaultedDestructor() = default;
};

class DefinesDefaultedDestructorOutside {
  ~DefinesDefaultedDestructorOutside();
};

DefinesDefaultedDestructorOutside::~DefinesDefaultedDestructorOutside() = default;

class DefinesCopyConstructor {
  DefinesCopyConstructor(const DefinesCopyConstructor &);
};
// CHECK-MESSAGES: [[@LINE-3]]:7: warning: class 'DefinesCopyConstructor' defines a copy constructor but does not define a destructor or a copy assignment operator [cppcoreguidelines-special-member-functions]

class DefinesCopyAssignment {
  DefinesCopyAssignment &operator=(const DefinesCopyAssignment &);
};
// CHECK-MESSAGES: [[@LINE-3]]:7: warning: class 'DefinesCopyAssignment' defines a copy assignment operator but does not define a destructor or a copy constructor [cppcoreguidelines-special-member-functions]

class DefinesMoveConstructor {
  DefinesMoveConstructor(DefinesMoveConstructor &&);
};
// CHECK-MESSAGES: [[@LINE-3]]:7: warning: class 'DefinesMoveConstructor' defines a move constructor but does not define a destructor or a move assignment operator [cppcoreguidelines-special-member-functions]

class DefinesMoveAssignment {
  DefinesMoveAssignment &operator=(DefinesMoveAssignment &&);
};
// CHECK-MESSAGES: [[@LINE-3]]:7: warning: class 'DefinesMoveAssignment' defines a move assignment operator but does not define a destructor or a move constructor [cppcoreguidelines-special-member-functions]

class DefinesNothing {
};

class DefinesEverything {
  DefinesEverything(const DefinesEverything &);
  DefinesEverything &operator=(const DefinesEverything &);
  DefinesEverything(DefinesEverything &&);
  DefinesEverything &operator=(DefinesEverything &&);
  ~DefinesEverything();
};

class DeletesEverything {
  DeletesEverything(const DeletesEverything &) = delete;
  DeletesEverything &operator=(const DeletesEverything &) = delete;
  DeletesEverything(DeletesEverything &&) = delete;
  DeletesEverything &operator=(DeletesEverything &&) = delete;
  ~DeletesEverything() = delete;
};

class DeletesCopyDefaultsMove {
  DeletesCopyDefaultsMove(const DeletesCopyDefaultsMove &) = delete;
  DeletesCopyDefaultsMove &operator=(const DeletesCopyDefaultsMove &) = delete;
  DeletesCopyDefaultsMove(DeletesCopyDefaultsMove &&) = default;
  DeletesCopyDefaultsMove &operator=(DeletesCopyDefaultsMove &&) = default;
  ~DeletesCopyDefaultsMove() = default;
};

template <typename T>
struct TemplateClass {
  TemplateClass() = default;
  TemplateClass(const TemplateClass &);
  TemplateClass &operator=(const TemplateClass &);
  TemplateClass(TemplateClass &&);
  TemplateClass &operator=(TemplateClass &&);
  ~TemplateClass();
};

// Multiple instantiations of a class template will trigger multiple matches for defined special members.
// This should not cause problems.
TemplateClass<int> InstantiationWithInt;
TemplateClass<double> InstantiationWithDouble;

struct NoCopy
{
  NoCopy() = default;
  ~NoCopy() = default;

  NoCopy(const NoCopy&) = delete;
  NoCopy(NoCopy&&) = delete;

  NoCopy& operator=(const NoCopy&) = delete;
  NoCopy& operator=(NoCopy&&) = delete;
};

// CHECK-MESSAGES: [[@LINE+1]]:8: warning: class 'NonCopyable' defines a copy constructor but does not define a destructor or a copy assignment operator [cppcoreguidelines-special-member-functions]
struct NonCopyable : NoCopy
{
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
};
